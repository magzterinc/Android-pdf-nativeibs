#include "mupdf/fitz.h"

void fz_register_document_handlers(fz_context *ctx)
{
	fz_register_document_handler(ctx, &pdf_document_handler);
}
