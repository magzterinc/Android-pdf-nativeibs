#include <stdio.h>
#include <stdlib.h>

#include "fitz-internal.h"
#include "mupdf-internal.h"

static pdf_obj *
resolve_dest_rec(pdf_document *xref, pdf_obj *dest, int depth)
{
	if (depth > 10) /* Arbitrary to avoid infinite recursion */
		return NULL;

	if (pdf_is_name(dest) || pdf_is_string(dest))
	{
		dest = pdf_lookup_dest(xref, dest);
		return resolve_dest_rec(xref, dest, depth+1);
	}

	else if (pdf_is_array(dest))
	{
		return dest;
	}

	else if (pdf_is_dict(dest))
	{
		dest = pdf_dict_gets(dest, "D");
		return resolve_dest_rec(xref, dest, depth+1);
	}

	else if (pdf_is_indirect(dest))
		return dest;

	return NULL;
}

static pdf_obj *
resolve_dest(pdf_document *xref, pdf_obj *dest)
{
	return resolve_dest_rec(xref, dest, 0);
}

fz_link_dest
pdf_parse_link_dest(pdf_document *xref, pdf_obj *dest)
{
	fz_link_dest ld;
	pdf_obj *obj;

	int l_from_2 = 0;
	int b_from_3 = 0;
	int r_from_4 = 0;
	int t_from_5 = 0;
	int t_from_3 = 0;
	int t_from_2 = 0;
	int z_from_4 = 0;

	dest = resolve_dest(xref, dest);
	if (dest == NULL || !pdf_is_array(dest))
	{
		ld.kind = FZ_LINK_NONE;
		return ld;
	}
	obj = pdf_array_get(dest, 0);
	if (pdf_is_int(obj))
		ld.ld.gotor.page = pdf_to_int(obj);
	else
		ld.ld.gotor.page = pdf_lookup_page_number(xref, obj);

	ld.kind = FZ_LINK_GOTO;
	ld.ld.gotor.flags = 0;
	ld.ld.gotor.lt.x = 0;
	ld.ld.gotor.lt.y = 0;
	ld.ld.gotor.rb.x = 0;
	ld.ld.gotor.rb.y = 0;
	ld.ld.gotor.file_spec = NULL;
	ld.ld.gotor.new_window = 0;

	obj = pdf_array_get(dest, 1);
	if (!pdf_is_name(obj))
		return ld;

	if (!strcmp("XYZ", pdf_to_name(obj)))
	{
		l_from_2 = t_from_3 = z_from_4 = 1;
		ld.ld.gotor.flags |= fz_link_flag_r_is_zoom;
	}
	else if ((!strcmp("Fit", pdf_to_name(obj))) || (!strcmp("FitB", pdf_to_name(obj))))
	{
		ld.ld.gotor.flags |= fz_link_flag_fit_h;
		ld.ld.gotor.flags |= fz_link_flag_fit_v;
	}
	else if ((!strcmp("FitH", pdf_to_name(obj))) || (!strcmp("FitBH", pdf_to_name(obj))))
	{
		t_from_2 = 1;
		ld.ld.gotor.flags |= fz_link_flag_fit_h;
	}
	else if ((!strcmp("FitV", pdf_to_name(obj))) || (!strcmp("FitBV", pdf_to_name(obj))))
	{
		l_from_2 = 1;
		ld.ld.gotor.flags |= fz_link_flag_fit_v;
	}
	else if (!strcmp("FitR", pdf_to_name(obj)))
	{
		l_from_2 = b_from_3 = r_from_4 = t_from_5 = 1;
		ld.ld.gotor.flags |= fz_link_flag_fit_h;
		ld.ld.gotor.flags |= fz_link_flag_fit_v;
	}

	if (l_from_2)
	{
		obj = pdf_array_get(dest, 2);
		if (pdf_is_int(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_l_valid;
			ld.ld.gotor.lt.x = pdf_to_int(obj);
		}
		else if (pdf_is_real(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_l_valid;
			ld.ld.gotor.lt.x = pdf_to_real(obj);
		}
	}
	if (b_from_3)
	{
		obj = pdf_array_get(dest, 3);
		if (pdf_is_int(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_b_valid;
			ld.ld.gotor.rb.y = pdf_to_int(obj);
		}
		else if (pdf_is_real(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_b_valid;
			ld.ld.gotor.rb.y = pdf_to_real(obj);
		}
	}
	if (r_from_4)
	{
		obj = pdf_array_get(dest, 4);
		if (pdf_is_int(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_r_valid;
			ld.ld.gotor.rb.x = pdf_to_int(obj);
		}
		else if (pdf_is_real(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_r_valid;
			ld.ld.gotor.rb.x = pdf_to_real(obj);
		}
	}
	if (t_from_5 || t_from_3 || t_from_2)
	{
		if (t_from_5)
			obj = pdf_array_get(dest, 5);
		else if (t_from_3)
			obj = pdf_array_get(dest, 3);
		else
			obj = pdf_array_get(dest, 2);
		if (pdf_is_int(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_t_valid;
			ld.ld.gotor.lt.y = pdf_to_int(obj);
		}
		else if (pdf_is_real(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_t_valid;
			ld.ld.gotor.lt.y = pdf_to_real(obj);
		}
	}
	if (z_from_4)
	{
		obj = pdf_array_get(dest, 4);
		if (pdf_is_int(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_r_valid;
			ld.ld.gotor.rb.x = pdf_to_int(obj);
		}
		else if (pdf_is_real(obj))
		{
			ld.ld.gotor.flags |= fz_link_flag_r_valid;
			ld.ld.gotor.rb.x = pdf_to_real(obj);
		}
	}

	/* Duplicate the values out for the sake of stupid clients */
	if ((ld.ld.gotor.flags & (fz_link_flag_l_valid | fz_link_flag_r_valid)) == fz_link_flag_l_valid)
		ld.ld.gotor.rb.x = ld.ld.gotor.lt.x;
	if ((ld.ld.gotor.flags & (fz_link_flag_l_valid | fz_link_flag_r_valid | fz_link_flag_r_is_zoom)) == fz_link_flag_r_valid)
		ld.ld.gotor.lt.x = ld.ld.gotor.rb.x;
	if ((ld.ld.gotor.flags & (fz_link_flag_t_valid | fz_link_flag_b_valid)) == fz_link_flag_t_valid)
		ld.ld.gotor.rb.y = ld.ld.gotor.lt.y;
	if ((ld.ld.gotor.flags & (fz_link_flag_t_valid | fz_link_flag_b_valid)) == fz_link_flag_b_valid)
		ld.ld.gotor.lt.y = ld.ld.gotor.rb.y;

	return ld;
}

fz_link_dest
pdf_parse_action(pdf_document *xref, pdf_obj *action)
{
	fz_link_dest ld;
	pdf_obj *obj, *dest;
	fz_context *ctx = xref->ctx;

	UNUSED(ctx);

	ld.kind = FZ_LINK_NONE;

	if (!action)
		return ld;

	obj = pdf_dict_gets(action, "S");
	if (!strcmp(pdf_to_name(obj), "GoTo"))
	{
		dest = pdf_dict_gets(action, "D");
		ld = pdf_parse_link_dest(xref, dest);
	}
	else if (!strcmp(pdf_to_name(obj), "URI"))
	{
		ld.kind = FZ_LINK_URI;
		ld.ld.uri.is_map = pdf_to_bool(pdf_dict_gets(action, "IsMap"));
		ld.ld.uri.uri = pdf_to_utf8(xref, pdf_dict_gets(action, "URI"));
	}
	else if (!strcmp(pdf_to_name(obj), "Launch"))
	{
		dest = pdf_dict_gets(action, "F");
		ld.kind = FZ_LINK_LAUNCH;
		if (pdf_is_dict(dest))
			dest = pdf_dict_gets(dest, "F");
		ld.ld.launch.file_spec = pdf_to_utf8(xref, dest);
		ld.ld.launch.new_window = pdf_to_int(pdf_dict_gets(action, "NewWindow"));
	}
	else if (!strcmp(pdf_to_name(obj), "Named"))
	{
		ld.kind = FZ_LINK_NAMED;
		ld.ld.named.named = pdf_to_utf8(xref, pdf_dict_gets(action, "N"));
	}
	else if (!strcmp(pdf_to_name(obj), "GoToR"))
	{
		dest = pdf_dict_gets(action, "D");
		ld = pdf_parse_link_dest(xref, dest);
		ld.kind = FZ_LINK_GOTOR;
		ld.ld.gotor.file_spec = pdf_to_utf8(xref, pdf_dict_gets(action, "F"));
		ld.ld.gotor.new_window = pdf_to_int(pdf_dict_gets(action, "NewWindow"));
	}
	return ld;
}

static fz_link *
pdf_load_link(pdf_document *xref, pdf_obj *dict, fz_matrix page_ctm)
{
	pdf_obj *dest = NULL;
	pdf_obj *action;
	pdf_obj *assetobj,*namesobj,*onetagobj,*efobjtag,*ftagobj;
	pdf_obj *obj;
	fz_rect bbox;
	fz_context *ctx = xref->ctx;
	fz_link_dest ld;
	int arraycount,i,loopcount=0;
	fz_buffer *strmbuf[3];
	char *filename[3];

	dest = NULL;

	obj = pdf_dict_gets(dict, "Rect");
	if (obj)
		bbox = pdf_to_rect(ctx, obj);
	else
		bbox = fz_empty_rect;

	bbox = fz_transform_rect(page_ctm, bbox);

	obj = pdf_dict_gets(dict, "Dest");
	if (obj)
	{
		dest = resolve_dest(xref, obj);
		ld = pdf_parse_link_dest(xref, dest);
	}
	else
	{
		action = pdf_dict_gets(dict, "A");
		
		if(action)
			ld = pdf_parse_action(xref, action);
		else
		{
			action = pdf_dict_gets(dict, "RichMediaContent");
			if(action)
			{
				assetobj = pdf_dict_gets(action, "Assets");
				if(assetobj)
				{
					namesobj = pdf_dict_gets(assetobj, "Names");
					if(namesobj)
					{
						arraycount = pdf_array_len(namesobj);
						for(i=0; i<arraycount; i++)
						{
							onetagobj = pdf_array_get(namesobj, i);
							if(onetagobj)
							{
								efobjtag = pdf_dict_gets(onetagobj, "EF");
								if(efobjtag)
								{
									ftagobj = pdf_dict_gets(efobjtag, "F");
									if(ftagobj)
									{
										if(pdf_is_stream(xref, pdf_to_num(ftagobj), pdf_to_gen(ftagobj)))
										{
											filename[loopcount] = pdf_to_utf8(xref, pdf_dict_gets(onetagobj, "F"));
											strmbuf[loopcount] = pdf_load_stream(xref, pdf_to_num(ftagobj), pdf_to_gen(ftagobj));
											loopcount++;
										}
									}
								}
							}
						}
						if(arraycount==6)
						{
							if(strmbuf[0]->len>strmbuf[1]->len && strmbuf[0]->len>strmbuf[2]->len)
							{
								ld.kind = FZ_LINK_MEDIA;
								ld.ld.media.filename = filename[0];
								ld.ld.media.mediastream = strmbuf[0];
							}
							else if(strmbuf[1]->len>strmbuf[2]->len)
							{
								ld.kind = FZ_LINK_MEDIA;
								ld.ld.media.filename = filename[1];
								ld.ld.media.mediastream = strmbuf[1];
							}
							else
							{
								ld.kind = FZ_LINK_MEDIA;
								ld.ld.media.filename = filename[2];
								ld.ld.media.mediastream = strmbuf[2];
							}
						}
						else
						{
							if(strmbuf[0]->len>strmbuf[1]->len)
							{
								ld.kind = FZ_LINK_MEDIA;
								ld.ld.media.filename = filename[0];
								ld.ld.media.mediastream = strmbuf[0];
							}
							else
							{
								ld.kind = FZ_LINK_MEDIA;
								ld.ld.media.filename = filename[1];
								ld.ld.media.mediastream = strmbuf[1];
							}
						}
					}
				}
			}	
		}
	}
	
	if (ld.kind == FZ_LINK_NONE)
		return NULL;
	return fz_new_link(ctx, bbox, ld);
}

fz_link *
pdf_load_link_annots(pdf_document *xref, pdf_obj *annots, fz_matrix page_ctm)
{
	LOGE("*******pdf_load_link_annots is called*******");
	
	fz_link *link, *head, *tail;
	pdf_obj *obj;
	int i, n;

	head = tail = NULL;
	link = NULL;

	n = pdf_array_len(annots);
	for (i = 0; i < n; i++)
	{
		obj = pdf_array_get(annots, i);
		link = pdf_load_link(xref, obj, page_ctm);
		if (link)
		{
			if (!head)
				head = tail = link;
			else
			{
				tail->next = link;
				tail = link;
			}
		}
	}

	return head;
}

void
pdf_free_annot(fz_context *ctx, pdf_annot *annot)
{
	pdf_annot *next;

	do
	{
		next = annot->next;
		if (annot->ap)
			pdf_drop_xobject(ctx, annot->ap);
		pdf_drop_obj(annot->obj);
		fz_free(ctx, annot);
		annot = next;
	}
	while (annot);
}

static void
pdf_transform_annot(pdf_annot *annot)
{
	fz_matrix matrix = annot->ap->matrix;
	fz_rect bbox = annot->ap->bbox;
	fz_rect rect = annot->rect;
	float w, h, x, y;

	bbox = fz_transform_rect(matrix, bbox);
	if (bbox.x1 == bbox.x0)
		w = 0;
	else
		w = (rect.x1 - rect.x0) / (bbox.x1 - bbox.x0);
	if (bbox.y1 == bbox.y0)
		h = 0;
	else
		h = (rect.y1 - rect.y0) / (bbox.y1 - bbox.y0);
	x = rect.x0 - bbox.x0;
	y = rect.y0 - bbox.y0;

	annot->matrix = fz_concat(fz_scale(w, h), fz_translate(x, y));
}

pdf_annot *
pdf_load_annots(pdf_document *xref, pdf_obj *annots, fz_matrix page_ctm)
{
	pdf_annot *annot, *head, *tail;
	pdf_obj *obj, *ap, *as, *n, *rect;
	int i, len;
	fz_context *ctx = xref->ctx;

	head = tail = NULL;
	annot = NULL;

	len = pdf_array_len(annots);
	for (i = 0; i < len; i++)
	{
		obj = pdf_array_get(annots, i);

		pdf_update_appearance(xref, obj);

		rect = pdf_dict_gets(obj, "Rect");
		ap = pdf_dict_gets(obj, "AP");
		as = pdf_dict_gets(obj, "AS");

		if (pdf_is_dict(ap))
		{
			pdf_hotspot *hp = &xref->hotspot;

			n = NULL;

			if (hp->num == pdf_to_num(obj)
				&& hp->gen == pdf_to_gen(obj)
				&& (hp->state & HOTSPOT_POINTER_DOWN))
			{
				n = pdf_dict_gets(ap, "D"); /* down state */
			}

			if (n == NULL)
				n = pdf_dict_gets(ap, "N"); /* normal state */

			/* lookup current state in sub-dictionary */
			if (!pdf_is_stream(xref, pdf_to_num(n), pdf_to_gen(n)))
				n = pdf_dict_get(n, as);


			annot = fz_malloc_struct(ctx, pdf_annot);
			annot->obj = pdf_keep_obj(obj);
			annot->rect = pdf_to_rect(ctx, rect);
			annot->pagerect = fz_transform_rect(page_ctm, annot->rect);
			annot->ap = NULL;
			annot->type = pdf_field_type(xref, obj);

			if (pdf_is_stream(xref, pdf_to_num(n), pdf_to_gen(n)))
			{
				fz_try(ctx)
				{
					annot->ap = pdf_load_xobject(xref, n);
					pdf_transform_annot(annot);
				}
				fz_catch(ctx)
				{
					fz_warn(ctx, "ignoring broken annotation");
				}
			}

			annot->next = NULL;

			if (obj == xref->focus_obj)
				xref->focus = annot;

			if (!head)
				head = tail = annot;
			else
			{
				tail->next = annot;
				tail = annot;
			}
		}
	}

	return head;
}

pdf_annot *
pdf_first_annot(pdf_document *doc, pdf_page *page)
{
	return page ? page->annots : NULL;
}

pdf_annot *
pdf_next_annot(pdf_document *doc, pdf_annot *annot)
{
	return annot ? annot->next : NULL;
}

fz_rect
pdf_bound_annot(pdf_document *doc, pdf_annot *annot)
{
	if (annot)
		return annot->pagerect;
	return fz_empty_rect;
}
