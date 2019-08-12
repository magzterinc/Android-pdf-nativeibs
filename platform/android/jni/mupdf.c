#include <jni.h>
#include <time.h>
#include <pthread.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef NDK_PROFILER
#include "prof.h"
#endif

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

#define JNI_FN(A) Java_com_artifex_mupdf_ ## A
#define PACKAGENAME "com/artifex/mupdf"

#define LOG_TAG "libmupdf"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGT(...) __android_log_print(ANDROID_LOG_INFO,"alert",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/* Enable to log rendering times (render each frame 100 times and time) */
#undef TIME_DISPLAY_LIST

#define MAX_SEARCH_HITS (500)
#define NUM_CACHE (3)
#define STRIKE_HEIGHT (0.375f)
#define UNDERLINE_HEIGHT (0.075f)
#define LINE_THICKNESS (0.07f)
#define INK_THICKNESS (4.0f)
#define SMALL_FLOAT (0.00001)
#define PROOF_RESOLUTION (300)

enum
{
	NONE,
	TEXT,
	LISTBOX,
	COMBOBOX,
	SIGNATURE
};

typedef struct rect_node_s rect_node;

struct rect_node_s
{
	fz_rect rect;
	rect_node *next;
};

typedef struct
{
	int number;
	int width;
	int height;
	fz_rect media_box;
	fz_page *page;
	rect_node *changed_rects;
	rect_node *hq_changed_rects;
	fz_display_list *page_list;
	fz_display_list *annot_list;
} page_cache;

typedef struct globals_s globals;

struct globals_s
{
	fz_colorspace *colorspace;
	fz_document *doc[NUM_CACHE];
	int resolution;
	fz_context *ctx;
	fz_rect *hit_bbox;
	int current;
	char *current_path;
	char *password;

	page_cache pages[NUM_CACHE];
	
	fz_document *imageDoc;
	page_cache imagePage;

	int alerts_initialised;
	// fin_lock and fin_lock2 are used during shutdown. The two waiting tasks
	// show_alert and waitForAlertInternal respectively take these locks while
	// waiting. During shutdown, the conditions are signaled and then the fin_locks
	// are taken momentarily to ensure the blocked threads leave the controlled
	// area of code before the mutexes and condition variables are destroyed.
	pthread_mutex_t fin_lock;
	pthread_mutex_t fin_lock2;
	// alert_lock is the main lock guarding the variables directly below.
	pthread_mutex_t alert_lock;
	// Flag indicating if the alert system is active. When not active, both
	// show_alert and waitForAlertInternal return immediately.
	int alerts_active;
	// Pointer to the alert struct passed in by show_alert, and valid while
	// show_alert is blocked.
	pdf_alert_event *current_alert;
	// Flag and condition varibles to signal a request is present and a reply
	// is present, respectively. The condition variables alone are not sufficient
	// because of the pthreads permit spurious signals.
	int alert_request;
	int alert_reply;
	pthread_cond_t alert_request_cond;
	pthread_cond_t alert_reply_cond;

	// For the buffer reading mode, we need to implement stream reading, which
	// needs access to the following.
	JNIEnv *env;
	jclass thiz;
};

static jfieldID global_fid;
static jfieldID buffer_fid;

// Do our best to avoid casting warnings.
#define CAST(type, var) (type)pointer_cast(var)

static inline void *pointer_cast(jlong l)
{
	return (void *)(intptr_t)l;
}

static inline jlong jlong_cast(void *p)
{
	return (jlong)(intptr_t)p;
}

static void drop_changed_rects(fz_context *ctx, rect_node **nodePtr)
{
	rect_node *node = *nodePtr;
	while (node)
	{
		rect_node *tnode = node;
		node = node->next;
		fz_free(ctx, tnode);
	}

	*nodePtr = NULL;
}

static void drop_page_cache(globals *glo, page_cache *pc, int i)
{
	fz_context *ctx = glo->ctx;
	//for (i = 0; i < NUM_CACHE; i++){
	fz_document *doc = glo->doc[i];

	if(pc != NULL)
	{
		//LOGI("Drop page %d", pc->number);
		if(pc->page != NULL)
		{
			fz_drop_display_list(ctx, pc->page_list);
			pc->page_list = NULL;
			fz_drop_display_list(ctx, pc->annot_list);
			pc->annot_list = NULL;
			fz_drop_page(ctx, pc->page);
			pc->page = NULL;
		}
	}
	
	if(doc != NULL){
	fz_drop_document(glo->ctx, doc);
	doc = NULL;
	}
	//}
}

static void drop_page_cache_image(globals *glo, page_cache *pc, fz_document *imageDoc)
{
	fz_context *ctx = glo->ctx;
	fz_document *doc = imageDoc;
	if(pc != NULL)
	{
		//LOGI("Drop page %d", pc->number);
		if(pc->page != NULL)
		{
			fz_drop_display_list(ctx, pc->page_list);
			pc->page_list = NULL;
			fz_drop_display_list(ctx, pc->annot_list);
			pc->annot_list = NULL;
			fz_drop_page(ctx, pc->page);
			pc->page = NULL;
		}
	}
	
	if(doc != NULL)
	{
		fz_drop_document(glo->ctx, doc);
		doc = NULL;
	}
}

static void dump_annotation_display_lists(globals *glo)
{
	fz_context *ctx = glo->ctx;
	int i;

	for (i = 0; i < NUM_CACHE; i++) {
		fz_drop_display_list(ctx, glo->pages[i].annot_list);
		glo->pages[i].annot_list = NULL;
	}
}

static void show_alert(globals *glo, pdf_alert_event *alert)
{
	pthread_mutex_lock(&glo->fin_lock2);
	pthread_mutex_lock(&glo->alert_lock);

	//LOGT("Enter show_alert: %s", alert->title);
	alert->button_pressed = 0;

	if (glo->alerts_active)
	{
		glo->current_alert = alert;
		glo->alert_request = 1;
		pthread_cond_signal(&glo->alert_request_cond);

		while (glo->alerts_active && !glo->alert_reply)
			pthread_cond_wait(&glo->alert_reply_cond, &glo->alert_lock);
		glo->alert_reply = 0;
		glo->current_alert = NULL;
	}

	//LOGT("Exit show_alert");

	pthread_mutex_unlock(&glo->alert_lock);
	pthread_mutex_unlock(&glo->fin_lock2);
}

static void event_cb(fz_context *ctx, pdf_document *doc, pdf_doc_event *event, void *data)
{
	globals *glo = (globals *)data;

	switch (event->type)
	{
	case PDF_DOCUMENT_EVENT_ALERT:
		show_alert(glo, pdf_access_alert_event(ctx, event));
		break;
	}
}

static void alerts_init(globals *glo)
{
	fz_context *ctx = glo->ctx;
	pdf_document *idoc = pdf_specifics(ctx, glo->doc[glo->current]);

	if (!idoc || glo->alerts_initialised)
		return;

	if (idoc)
		pdf_enable_js(ctx, idoc);

	glo->alerts_active = 0;
	glo->alert_request = 0;
	glo->alert_reply = 0;
	pthread_mutex_init(&glo->fin_lock, NULL);
	pthread_mutex_init(&glo->fin_lock2, NULL);
	pthread_mutex_init(&glo->alert_lock, NULL);
	pthread_cond_init(&glo->alert_request_cond, NULL);
	pthread_cond_init(&glo->alert_reply_cond, NULL);

	pdf_set_doc_event_callback(ctx, idoc, event_cb, glo);
	//LOGT("alert_init");
	glo->alerts_initialised = 1;
}

static void alerts_fin(globals *glo)
{
	fz_context *ctx = glo->ctx;
	pdf_document *idoc = pdf_specifics(ctx, glo->doc[glo->current]);
	if (!glo->alerts_initialised)
		return;

	//LOGT("Enter alerts_fin");
	if (idoc)
		pdf_set_doc_event_callback(ctx, idoc, NULL, NULL);

	// Set alerts_active false and wake up show_alert and waitForAlertInternal,
	pthread_mutex_lock(&glo->alert_lock);
	glo->current_alert = NULL;
	glo->alerts_active = 0;
	pthread_cond_signal(&glo->alert_request_cond);
	pthread_cond_signal(&glo->alert_reply_cond);
	pthread_mutex_unlock(&glo->alert_lock);

	// Wait for the fin_locks.
	pthread_mutex_lock(&glo->fin_lock);
	pthread_mutex_unlock(&glo->fin_lock);
	pthread_mutex_lock(&glo->fin_lock2);
	pthread_mutex_unlock(&glo->fin_lock2);

	pthread_cond_destroy(&glo->alert_reply_cond);
	pthread_cond_destroy(&glo->alert_request_cond);
	pthread_mutex_destroy(&glo->alert_lock);
	pthread_mutex_destroy(&glo->fin_lock2);
	pthread_mutex_destroy(&glo->fin_lock);
	//LOGT("Exit alerts_fin");
	glo->alerts_initialised = 0;
}

// Should only be called from the single background AsyncTask thread
static globals *get_globals(JNIEnv *env, jobject thiz)
{
	globals *glo = CAST(globals *, (*env)->GetLongField(env, thiz, global_fid));
	if (glo != NULL)
	{
		glo->env = env;
		glo->thiz = thiz;
	}
	return glo;
}

// May be called from any thread, provided the values of glo->env and glo->thiz
// are not used.
static globals *get_globals_any_thread(JNIEnv *env, jobject thiz)
{
	return (globals *)(intptr_t)((*env)->GetLongField(env, thiz, global_fid));
}

JNIEXPORT jlong JNICALL
JNI_FN(MuPDFCore_openFile)(JNIEnv * env, jobject thiz, jstring jfilename, jstring jpassword)
{
	const char *filename;
	const char *pw;
	globals *glo;
	fz_context *ctx;
	jclass clazz;

#ifdef NDK_PROFILER
	monstartup("libmagzter.so");
#endif

	clazz = (*env)->GetObjectClass(env, thiz);
	global_fid = (*env)->GetFieldID(env, clazz, "globals", "J");

	glo = calloc(1, sizeof(*glo));
	if (glo == NULL)
		return 0;
	glo->resolution = 144;
	glo->alerts_initialised = 0;

#ifdef DEBUG
	/* Try and send stdout/stderr to file in debug builds. This
	 * path may not work on all platforms, but it works on the
	 * LG G3, and it's no worse than not redirecting it anywhere
	 * on anything else. */
	freopen("/storage/emulated/0/Download/stdout.txt", "a", stdout);
	freopen("/storage/emulated/0/Download/stderr.txt", "a", stderr);
#endif

	filename = (*env)->GetStringUTFChars(env, jfilename, NULL);
	pw = (*env)->GetStringUTFChars(env, jpassword, NULL);
	if (filename == NULL)
	{
		//LOGE("Failed to get filename");
		free(glo);
		return 0;
	}

	/* 128 MB store for low memory devices. Tweak as necessary. */
	glo->ctx = ctx = fz_new_context(NULL, NULL, 128 << 20);
	if (!ctx)
	{
		//LOGE("Failed to initialise context");
		(*env)->ReleaseStringUTFChars(env, jfilename, filename);
		(*env)->ReleaseStringUTFChars(env, jpassword, pw);
		free(glo);
		return 0;
	}

	fz_register_document_handlers(ctx);

	//glo->doc = NULL;
	fz_try(ctx)
	{
		glo->colorspace = fz_device_rgb(ctx);

		//LOGI("Opening document...");
		fz_try(ctx)
		{
			glo->current_path = fz_strdup(ctx, (char *)filename);
			glo->password = fz_strdup(ctx, (char *)pw);
			alerts_init(glo);
		}
		fz_catch(ctx)
		{
			fz_throw(ctx, FZ_ERROR_GENERIC, "Cannot open document: '%s'", filename);
		}
		//LOGI("Done!");
	}
	fz_catch(ctx)
	{
		//LOGE("Failed: %s", ctx->error->message);
		//fz_drop_document(ctx, glo->doc);
		//glo->doc = NULL;
		fz_drop_context(ctx);
		glo->ctx = NULL;
		free(glo);
		glo = NULL;
	}

	(*env)->ReleaseStringUTFChars(env, jfilename, filename);
	(*env)->ReleaseStringUTFChars(env, jpassword, pw);

	return jlong_cast(glo);
}

JNIEXPORT void JNICALL
JNI_FN(MuPDFCore_gotoPageInternalImage)(JNIEnv *env, jobject thiz, int page)
{
	float zoom;
	fz_matrix ctm;
	fz_irect bbox;
	page_cache *pc;
	globals *glo = get_globals(env, thiz);
	if (glo == NULL)
		return;
	fz_context *ctx = glo->ctx;
	
	char intBuffer[5];
	char *path;
	char *lastSubString = ".pdf";
	jboolean isPasswordNeeded;
	
	if (glo->imagePage.page != NULL && glo->imagePage.number == page)
	{
		/* The page is already cached */
		return;
	}

	pc = &glo->imagePage;

	drop_page_cache_image(glo, pc, glo->imageDoc);
	
	sprintf(intBuffer, "%d", (page));
	path = (char*) malloc(strlen(glo->current_path)+strlen(intBuffer)+strlen(lastSubString)+1);
	path[0] = '\0';
	strcat(path,glo->current_path);
	strcat(path,intBuffer);
	strcat(path,lastSubString);
	
	//LOGI("Goto page %s...", path);
	//LOGI("Password %s...", glo->password);
	
	fz_try(ctx)
	{
		glo->imageDoc = fz_open_document(ctx, path);
			
		isPasswordNeeded = fz_needs_password(ctx, glo->imageDoc) ? JNI_TRUE : JNI_FALSE;
		if(isPasswordNeeded)
		{
			fz_authenticate_password(ctx, glo->imageDoc, glo->password);
		}
		
		//LOGI("Password validation done");
		
		if(path != NULL)
		{
			free(path);		
			path = NULL;
		}
	}
	fz_catch(ctx)
	{
		//LOGE("cannot make displaylist from page %d", pc->number);
	}

	/* In the event of an error, ensure we give a non-empty page */
	pc->width = 100;
	pc->height = 100;

	pc->number = page;
	//LOGI("Goto page %d...", page);
	fz_try(ctx)
	{
		fz_rect rect;
		//LOGI("Load page %d", pc->number);
		pc->page = fz_load_page(ctx, glo->imageDoc, 0);
		zoom = glo->resolution / 72;
		fz_bound_page(ctx, pc->page, &pc->media_box);
		fz_scale(&ctm, zoom, zoom);
		rect = pc->media_box;
		fz_round_rect(&bbox, fz_transform_rect(&rect, &ctm));
		pc->width = bbox.x1-bbox.x0;
		pc->height = bbox.y1-bbox.y0;
	}
	fz_catch(ctx)
	{
		//LOGE("cannot make displaylist from page %d", pc->number);
	}
}

JNIEXPORT void JNICALL
JNI_FN(MuPDFCore_gotoPageInternal)(JNIEnv *env, jobject thiz, int page)
{
	int i;
	int furthest;
	int furthest_dist = -1;
	float zoom;
	fz_matrix ctm;
	fz_irect bbox;
	page_cache *pc;
	globals *glo = get_globals(env, thiz);
	if (glo == NULL)
		return;
	fz_context *ctx = glo->ctx;
	
	char intBuffer[5];
	char *path;
	char *lastSubString = ".pdf";
	jboolean isPasswordNeeded;

	for (i = 0; i < NUM_CACHE; i++)
	{
		if (glo->pages[i].page != NULL && glo->pages[i].number == page)
		{
			/* The page is already cached */
			glo->current = i;
			return;
		}

		if (glo->pages[i].page == NULL)
		{
			/* cache record unused, and so a good one to use */
			furthest = i;
			furthest_dist = INT_MAX;
		}
		else
		{
			int dist = abs(glo->pages[i].number - page);

			/* Further away - less likely to be needed again */
			if (dist > furthest_dist)
			{
				furthest_dist = dist;
				furthest = i;
			}
		}
	}

	glo->current = furthest;
	
	pc = &glo->pages[glo->current];

	drop_page_cache(glo, pc, glo->current);
	
	sprintf(intBuffer, "%d", (page));
	path = (char*)malloc(strlen(glo->current_path)+strlen(intBuffer)+strlen(lastSubString) + 1);
	path[0] = '\0';
	strcat(path,glo->current_path);
	strcat(path,intBuffer);
	strcat(path,lastSubString);
	
	//LOGI("Goto page %s...", path);
	//LOGI("Password %s...", glo->password);
	
	fz_try(ctx)
	{
		glo->doc[glo->current] = fz_open_document(ctx, path);
			
		isPasswordNeeded = fz_needs_password(ctx, glo->doc[glo->current]) ? JNI_TRUE : JNI_FALSE;
		if(isPasswordNeeded)
		{
			fz_authenticate_password(ctx, glo->doc[glo->current], glo->password);
		}
		//LOGI("Password validation done");
		if(path != NULL)
		{
			free(path);	
			path = NULL;
		}
	}
	fz_catch(ctx)
	{
		//LOGE("cannot make displaylist from page %d", pc->number);
	}
	
	/* In the event of an error, ensure we give a non-empty page */
	pc->width = 100;
	pc->height = 100;

	pc->number = page;
	//LOGI("Goto page %d...", page);
	fz_try(ctx)
	{
		fz_rect rect;
		//LOGI("Load page %d", pc->number);
		pc->page = fz_load_page(ctx, glo->doc[glo->current], 0);
		zoom = glo->resolution / 72;
		fz_bound_page(ctx, pc->page, &pc->media_box);
		fz_scale(&ctm, zoom, zoom);
		rect = pc->media_box;
		fz_round_rect(&bbox, fz_transform_rect(&rect, &ctm));
		pc->width = bbox.x1-bbox.x0;
		pc->height = bbox.y1-bbox.y0;
	}
	fz_catch(ctx)
	{
		//LOGE("cannot make displaylist from page %d", pc->number);
	}
}

JNIEXPORT float JNICALL
JNI_FN(MuPDFCore_getPageWidth)(JNIEnv *env, jobject thiz, int forImage)
{
	globals *glo = get_globals(env, thiz);
	if(forImage == 0)
	{
		//LOGI("PageWidth=%d", glo->pages[glo->current].width);
		return glo->pages[glo->current].width;
	}
	else
	{
		//LOGI("PageWidth=%d", glo->imagePage.width);
		return glo->imagePage.width;
	}
}

JNIEXPORT float JNICALL
JNI_FN(MuPDFCore_getPageHeight)(JNIEnv *env, jobject thiz, int forImage)
{
	globals *glo = get_globals(env, thiz);
	if(forImage == 0)
	{
		//LOGI("PageHeight=%d", glo->pages[glo->current].height);
		return glo->pages[glo->current].height;
	}
	else
	{
		//LOGI("PageWidth=%d", glo->imagePage.height);
		return glo->imagePage.height;
	}
}

static void update_changed_rects(globals *glo, page_cache *pc, pdf_document *idoc)
{
	fz_context *ctx = glo->ctx;
	fz_annot *annot;

	pdf_update_page(ctx, idoc, (pdf_page *)pc->page);
	while ((annot = (fz_annot *)pdf_poll_changed_annot(ctx, idoc, (pdf_page *)pc->page)) != NULL)
	{
		/* FIXME: We bound the annot twice here */
		rect_node *node = fz_malloc_struct(glo->ctx, rect_node);
		fz_bound_annot(ctx, annot, &node->rect);
		node->next = pc->changed_rects;
		pc->changed_rects = node;

		node = fz_malloc_struct(glo->ctx, rect_node);
		fz_bound_annot(ctx, annot, &node->rect);
		node->next = pc->hq_changed_rects;
		pc->hq_changed_rects = node;
	}
}

JNIEXPORT jboolean JNICALL
JNI_FN(MuPDFCore_drawPage)(JNIEnv *env, jobject thiz, jobject bitmap,
		int pageW, int pageH, int patchX, int patchY, int patchW, int patchH, jlong cookiePtr)
{
	AndroidBitmapInfo info;
	void *pixels;
	int ret;
	fz_device *dev = NULL;
	float zoom;
	fz_matrix ctm;
	fz_irect bbox;
	fz_rect rect;
	fz_pixmap *pix = NULL;
	float xscale, yscale;
	globals *glo = get_globals(env, thiz);
	fz_context *ctx = glo->ctx;
	fz_document *doc = glo->imageDoc;
	page_cache *pc = &glo->imagePage;
	int hq = (patchW < pageW || patchH < pageH);
	fz_matrix scale;
	fz_cookie *cookie = (fz_cookie *)(intptr_t)cookiePtr;

	if (pc->page == NULL)
		return 0;

	fz_var(pix);
	fz_var(dev);

	//LOGI("In native method\n");
	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
		//LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return 0;
	}

	//LOGI("Checking format\n");
	if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		//LOGE("Bitmap format is not RGBA_8888 !");
		return 0;
	}

	//LOGI("locking pixels\n");
	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
		//LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
		return 0;
	}

	/* Call mupdf to render display list to screen */
	//LOGI("Rendering page(%d)=%dx%d patch=[%d,%d,%d,%d]",pc->number, pageW, pageH, patchX, patchY, patchW, patchH);

	fz_try(ctx)
	{
		fz_irect pixbbox;

		if (pc->page_list == NULL)
		{
			/* Render to list */
			pc->page_list = fz_new_display_list(ctx);
			dev = fz_new_list_device(ctx, pc->page_list);
			fz_run_page_contents(ctx, pc->page, dev, &fz_identity, cookie);
			fz_drop_device(ctx, dev);
			dev = NULL;
			if (cookie != NULL && cookie->abort)
			{
				fz_drop_display_list(ctx, pc->page_list);
				pc->page_list = NULL;
				fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");
			}
		}
		if (pc->annot_list == NULL)
		{
			fz_annot *annot;
			pc->annot_list = fz_new_display_list(ctx);
			dev = fz_new_list_device(ctx, pc->annot_list);
			for (annot = fz_first_annot(ctx, pc->page); annot; annot = fz_next_annot(ctx, annot))
				fz_run_annot(ctx, annot, dev, &fz_identity, cookie);
			fz_drop_device(ctx, dev);
			dev = NULL;
			if (cookie != NULL && cookie->abort)
			{
				fz_drop_display_list(ctx, pc->annot_list);
				pc->annot_list = NULL;
				fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");
			}
		}
		bbox.x0 = patchX;
		bbox.y0 = patchY;
		bbox.x1 = patchX + patchW;
		bbox.y1 = patchY + patchH;
		pixbbox = bbox;
		pixbbox.x1 = pixbbox.x0 + info.width;
		/* pixmaps cannot handle right-edge padding, so the bbox must be expanded to
		 * match the pixels data */
		pix = fz_new_pixmap_with_bbox_and_data(ctx, glo->colorspace, &pixbbox, pixels);
		if (pc->page_list == NULL && pc->annot_list == NULL)
		{
			fz_clear_pixmap_with_value(ctx, pix, 0xd0);
			break;
		}
		fz_clear_pixmap_with_value(ctx, pix, 0xff);

		zoom = glo->resolution / 72;
		fz_scale(&ctm, zoom, zoom);
		rect = pc->media_box;
		fz_round_rect(&bbox, fz_transform_rect(&rect, &ctm));
		/* Now, adjust ctm so that it would give the correct page width
		 * heights. */
		xscale = (float)pageW/(float)(bbox.x1-bbox.x0);
		yscale = (float)pageH/(float)(bbox.y1-bbox.y0);
		fz_concat(&ctm, &ctm, fz_scale(&scale, xscale, yscale));
		rect = pc->media_box;
		fz_transform_rect(&rect, &ctm);
		dev = fz_new_draw_device(ctx, pix);
#ifdef TIME_DISPLAY_LIST
		{
			clock_t time;
			int i;

			//LOGI("Executing display list");
			time = clock();
			for (i=0; i<100;i++) {
#endif
				if (pc->page_list)
					fz_run_display_list(ctx, pc->page_list, dev, &ctm, &rect, cookie);
				if (cookie != NULL && cookie->abort)
					fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");

				if (pc->annot_list)
					fz_run_display_list(ctx, pc->annot_list, dev, &ctm, &rect, cookie);
				if (cookie != NULL && cookie->abort)
					fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");

#ifdef TIME_DISPLAY_LIST
			}
			time = clock() - time;
			//LOGI("100 renders in %d (%d per sec)", time, CLOCKS_PER_SEC);
		}
#endif
		fz_drop_device(ctx, dev);
		dev = NULL;
		fz_drop_pixmap(ctx, pix);
		//LOGI("Rendered");
	}
	fz_always(ctx)
	{
		fz_drop_device(ctx, dev);
		dev = NULL;
	}
	fz_catch(ctx)
	{
		//LOGE("Render failed");
	}

	AndroidBitmap_unlockPixels(env, bitmap);

	return 1;
}

JNIEXPORT jboolean JNICALL
JNI_FN(MuPDFCore_drawPageZoom)(JNIEnv *env, jobject thiz, jobject bitmap,
		int pageW, int pageH, int patchX, int patchY, int patchW, int patchH, jlong cookiePtr)
{
	AndroidBitmapInfo info;
	void *pixels;
	int ret;
	fz_device *dev = NULL;
	float zoom;
	fz_matrix ctm;
	fz_irect bbox;
	fz_rect rect;
	fz_pixmap *pix = NULL;
	float xscale, yscale;
	globals *glo = get_globals(env, thiz);
	fz_context *ctx = glo->ctx;
	fz_document *doc = glo->doc[glo->current];
	page_cache *pc = &glo->pages[glo->current];
	int hq = (patchW < pageW || patchH < pageH);
	fz_matrix scale;
	fz_cookie *cookie = (fz_cookie *)(intptr_t)cookiePtr;

	if (pc->page == NULL)
		return 0;

	fz_var(pix);
	fz_var(dev);

	//LOGI("In native method\n");
	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
		//LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return 0;
	}

	//LOGI("Checking format\n");
	if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		//LOGE("Bitmap format is not RGBA_8888 !");
		return 0;
	}

	//LOGI("locking pixels\n");
	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
		//LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
		return 0;
	}

	/* Call mupdf to render display list to screen */
	//LOGI("Rendering page(%d)=%dx%d patch=[%d,%d,%d,%d]",pc->number, pageW, pageH, patchX, patchY, patchW, patchH);

	fz_try(ctx)
	{
		fz_irect pixbbox;
		pdf_document *idoc = pdf_specifics(ctx, doc);

		if (idoc)
		{
			/* Update the changed-rects for both hq patch and main bitmap */
			update_changed_rects(glo, pc, idoc);

			/* Then drop the changed-rects for the bitmap we're about to
			render because we are rendering the entire area */
			drop_changed_rects(ctx, hq ? &pc->hq_changed_rects : &pc->changed_rects);
		}

		if (pc->page_list == NULL)
		{
			/* Render to list */
			pc->page_list = fz_new_display_list(ctx);
			dev = fz_new_list_device(ctx, pc->page_list);
			fz_run_page_contents(ctx, pc->page, dev, &fz_identity, cookie);
			fz_drop_device(ctx, dev);
			dev = NULL;
			if (cookie != NULL && cookie->abort)
			{
				fz_drop_display_list(ctx, pc->page_list);
				pc->page_list = NULL;
				fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");
			}
		}
		if (pc->annot_list == NULL)
		{
			fz_annot *annot;
			pc->annot_list = fz_new_display_list(ctx);
			dev = fz_new_list_device(ctx, pc->annot_list);
			for (annot = fz_first_annot(ctx, pc->page); annot; annot = fz_next_annot(ctx, annot))
				fz_run_annot(ctx, annot, dev, &fz_identity, cookie);
			fz_drop_device(ctx, dev);
			dev = NULL;
			if (cookie != NULL && cookie->abort)
			{
				fz_drop_display_list(ctx, pc->annot_list);
				pc->annot_list = NULL;
				fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");
			}
		}
		bbox.x0 = patchX;
		bbox.y0 = patchY;
		bbox.x1 = patchX + patchW;
		bbox.y1 = patchY + patchH;
		pixbbox = bbox;
		pixbbox.x1 = pixbbox.x0 + info.width;
		/* pixmaps cannot handle right-edge padding, so the bbox must be expanded to
		 * match the pixels data */
		pix = fz_new_pixmap_with_bbox_and_data(ctx, glo->colorspace, &pixbbox, pixels);
		if (pc->page_list == NULL && pc->annot_list == NULL)
		{
			fz_clear_pixmap_with_value(ctx, pix, 0xd0);
			break;
		}
		fz_clear_pixmap_with_value(ctx, pix, 0xff);

		zoom = glo->resolution / 72;
		fz_scale(&ctm, zoom, zoom);
		rect = pc->media_box;
		fz_round_rect(&bbox, fz_transform_rect(&rect, &ctm));
		/* Now, adjust ctm so that it would give the correct page width
		 * heights. */
		xscale = (float)pageW/(float)(bbox.x1-bbox.x0);
		yscale = (float)pageH/(float)(bbox.y1-bbox.y0);
		fz_concat(&ctm, &ctm, fz_scale(&scale, xscale, yscale));
		rect = pc->media_box;
		fz_transform_rect(&rect, &ctm);
		dev = fz_new_draw_device(ctx, pix);
#ifdef TIME_DISPLAY_LIST
		{
			clock_t time;
			int i;

			//LOGI("Executing display list");
			time = clock();
			for (i=0; i<100;i++) {
#endif
				if (pc->page_list)
					fz_run_display_list(ctx, pc->page_list, dev, &ctm, &rect, cookie);
				if (cookie != NULL && cookie->abort)
					fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");

				if (pc->annot_list)
					fz_run_display_list(ctx, pc->annot_list, dev, &ctm, &rect, cookie);
				if (cookie != NULL && cookie->abort)
					fz_throw(ctx, FZ_ERROR_GENERIC, "Render aborted");

#ifdef TIME_DISPLAY_LIST
			}
			time = clock() - time;
			//LOGI("100 renders in %d (%d per sec)", time, CLOCKS_PER_SEC);
		}
#endif
		fz_drop_device(ctx, dev);
		dev = NULL;
		fz_drop_pixmap(ctx, pix);
		//LOGI("Rendered");
	}
	fz_always(ctx)
	{
		fz_drop_device(ctx, dev);
		dev = NULL;
	}
	fz_catch(ctx)
	{
		//LOGE("Render failed");
	}

	AndroidBitmap_unlockPixels(env, bitmap);

	return 1;
}

/* Close the document, at least enough to be able to save over it. This
 * may be called again later, so must be idempotent. */
static void close_doc(globals *glo)
{
	int i;

	fz_free(glo->ctx, glo->hit_bbox);
	glo->hit_bbox = NULL;
	
	drop_page_cache_image(glo, &glo->imagePage, glo->imageDoc);

	for (i = 0; i < NUM_CACHE; i++){
		drop_page_cache(glo, &glo->pages[i], i);
	}

	alerts_fin(glo);
}

JNIEXPORT void JNICALL
JNI_FN(MuPDFCore_destroying)(JNIEnv * env, jobject thiz)
{
	globals *glo = get_globals(env, thiz);

	if (glo == NULL)
		return;
	//LOGI("Destroying");
	fz_free(glo->ctx, glo->current_path);
	glo->current_path = NULL;
	fz_drop_context(glo->ctx);
	glo->ctx = NULL;
	close_doc(glo);
	//free(glo);
#ifdef MEMENTO
	//LOGI("Destroying dump start");
	Memento_listBlocks();
	Memento_stats();
	//LOGI("Destroying dump end");
#endif
#ifdef NDK_PROFILER
	// Apparently we should really be writing to whatever path we get
	// from calling getFilesDir() in the java part, which supposedly
	// gives /sdcard/data/data/com.artifex.MuPDF/gmon.out, but that's
	// unfriendly.
	setenv("CPUPROFILE", "/sdcard/gmon.out", 1);
	moncleanup();
#endif
}

JNIEXPORT jstring JNICALL
JNI_FN(MuPDFCore_getMediaStream)(JNIEnv * env, jobject thiz, jstring fileName, int pageNumber)
{
	fz_matrix ctm;
	float zoom;
	fz_link *list;
	fz_link *link;
	page_cache *pc;
	globals *glo = get_globals(env, thiz);
	const char *sname;
	char *filePath;
	
	JNI_FN(MuPDFCore_gotoPageInternal)(env, thiz, pageNumber);
	pc = &glo->pages[glo->current];
	if (pc->page == NULL || pc->number != pageNumber)
		return NULL;
	
	sname = (*env)->GetStringUTFChars(env, fileName, NULL);
	//LOGI("FILE name %s", sname);

	zoom = glo->resolution / 72;
	fz_scale(&ctm, zoom, zoom);

	list = fz_load_links(glo->ctx, pc->page);
	
	for (link = list; link; link = link->next)
	{
		if(link->dest.kind == FZ_LINK_MEDIA)
		{	
			if(strcmp(sname,link->dest.ld.media.filename) == 0)
			{
				filePath = malloc(strlen(glo->current_path)+strlen(link->dest.ld.media.filename)+1);
				filePath[0] = '\0';
				strcat(filePath,glo->current_path);
				strcat(filePath,link->dest.ld.media.filename);
				
				FILE * pFile;
				pFile = fopen (filePath, "w");
				fwrite (link->dest.ld.media.mediastream->data, 1, link->dest.ld.media.mediastream->len, pFile);
				fclose (pFile);
				
				break;
			}
		}
	}
	
	(*env)->ReleaseStringUTFChars(env, fileName, sname);
	
	return (*env)->NewStringUTF(env, filePath);
}

JNIEXPORT jobjectArray JNICALL
JNI_FN(MuPDFCore_getPageLinksInternal)(JNIEnv * env, jobject thiz, int pageNumber, int forImage)
{
	jclass linkInfoClass;
	jmethodID ctorExternal;
	jobjectArray arr;
	jobject linkInfo;
	fz_matrix ctm;
	float zoom;
	fz_link *list;
	fz_link *link;
	int count;
	page_cache *pc;
	globals *glo = get_globals(env, thiz);

	linkInfoClass = (*env)->FindClass(env, PACKAGENAME "/LinkInfo");
	if (linkInfoClass == NULL) return NULL;
	
	ctorExternal = (*env)->GetMethodID(env, linkInfoClass, "<init>", "(FFFFLjava/lang/String;)V");
	if (ctorExternal == NULL) return NULL;
	

	if(forImage == 0)
	{
		JNI_FN(MuPDFCore_gotoPageInternal)(env, thiz, pageNumber);
		pc = &glo->pages[glo->current];
	}
	else
		pc = &glo->imagePage;
	
	
	if (pc->page == NULL || pc->number != pageNumber)
			return NULL;
	

	zoom = glo->resolution / 72;
	fz_scale(&ctm, zoom, zoom);

	list = fz_load_links(glo->ctx, pc->page);
	count = 0;
	for (link = list; link; link = link->next)
	{
		switch (link->dest.kind)
		{
		case FZ_LINK_URI:
		case FZ_LINK_MEDIA:
			count++ ;
		}
	}

	arr = (*env)->NewObjectArray(env, count, linkInfoClass, NULL);
	if (arr == NULL)
	{
		fz_drop_link(glo->ctx, list);
		return NULL;
	}

	count = 0;
	for (link = list; link; link = link->next)
	{
		fz_rect rect = link->rect;
		fz_transform_rect(&rect, &ctm);

		switch (link->dest.kind)
		{

		case FZ_LINK_URI:
		{
			jstring juri = (*env)->NewStringUTF(env, link->dest.ld.uri.uri);
			linkInfo = (*env)->NewObject(env, linkInfoClass, ctorExternal,
					(float)rect.x0, (float)rect.y0, (float)rect.x1, (float)rect.y1,
					juri);
			break;
		}
		
		case FZ_LINK_MEDIA:
		{
			char *name;
			char *startingWith = "MEDIA:";
			
			name = malloc(strlen(startingWith)+strlen(link->dest.ld.media.filename)+1);
			name[0] = '\0';
			strcat(name,startingWith);
			strcat(name,link->dest.ld.media.filename);
			
			jstring juri = (*env)->NewStringUTF(env, name);
			
			linkInfo = (*env)->NewObject(env, linkInfoClass, ctorExternal,
					(float)rect.x0, (float)rect.y0, (float)rect.x1, (float)rect.y1,
					juri);

			if(name != NULL)
			{
				free(name);
				name = NULL;
			}
			
			break;
		}

		default:
			continue;
		}

		if (linkInfo == NULL)
		{
			fz_drop_link(glo->ctx, list);
			return NULL;
		}
		(*env)->SetObjectArrayElement(env, arr, count, linkInfo);
		(*env)->DeleteLocalRef(env, linkInfo);
		count++;
	}
	fz_drop_link(glo->ctx, list);

	return arr;
}

JNIEXPORT void JNICALL
JNI_FN(MuPDFCore_dumpMemoryInternal)(JNIEnv * env, jobject thiz)
{
	globals *glo = get_globals(env, thiz);
	fz_context *ctx = glo->ctx;

#ifdef MEMENTO
	//LOGE("dumpMemoryInternal start");
	Memento_listNewBlocks();
	Memento_stats();
	//LOGE("dumpMemoryInternal end");
#endif
}

JNIEXPORT jlong JNICALL
JNI_FN(MuPDFCore_createCookie)(JNIEnv * env, jobject thiz)
{
	globals *glo = get_globals_any_thread(env, thiz);
	if (glo == NULL)
		return 0;
	fz_context *ctx = glo->ctx;

	return (jlong) (intptr_t) fz_calloc_no_throw(ctx,1, sizeof(fz_cookie));
}

JNIEXPORT void JNICALL
JNI_FN(MuPDFCore_destroyCookie)(JNIEnv * env, jobject thiz, jlong cookiePtr)
{
	fz_cookie *cookie = (fz_cookie *) (intptr_t) cookiePtr;
	globals *glo = get_globals_any_thread(env, thiz);
	if (glo == NULL)
		return;
	fz_context *ctx = glo->ctx;

	fz_free(ctx, cookie);
}

JNIEXPORT void JNICALL
JNI_FN(MuPDFCore_abortCookie)(JNIEnv * env, jobject thiz, jlong cookiePtr)
{
	fz_cookie *cookie = (fz_cookie *) (intptr_t) cookiePtr;
	if (cookie != NULL)
		cookie->abort = 1;
}
