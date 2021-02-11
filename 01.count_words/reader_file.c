#include "reader_file.h"
#include "macro.h"
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

static void reader_file_destroy_by_base_ptr(Reader *reader)
	{ reader_file_destroy((ReaderFile *) reader); }
static int reader_file_getc_by_base_ptr(Reader *reader)
	{ return reader_file_getc((ReaderFile *) reader); }
static int reader_file_ungetc_by_base_ptr(Reader *reader, int ch)
	{ return reader_file_ungetc((ReaderFile *) reader, ch); }


/*  If error occurs, returns NULL and errno is set appropriately.
 * In this case, object doesn't need to be deleted, but that won't be an error.
 *  On success, result should be destroyed after use with reader_destroy()
 * or reader_file_destroy() */
ReaderFile *reader_file_create(const char *filename)
{
	assert(filename);
	FILE *input_file;

	ALLOC(ReaderFile, reader);
	if ((input_file = fopen(filename, "r")) == NULL)
		return NULL;
	reader->getc = reader_file_getc_by_base_ptr;
	reader->ungetc = reader_file_ungetc_by_base_ptr;
	reader->destroy = reader_file_destroy_by_base_ptr;
	reader->input_file = input_file;
	return reader;
}

void reader_file_destroy(ReaderFile *reader)
{
	if (!reader)
		return;
	assert(reader->input_file != NULL);
	assert(reader->destroy != NULL);
	assert((void *) reader->destroy == (void *) reader_file_destroy_by_base_ptr
		&& "reader_file_destroy() invoked for not ReaderFile object");
	fclose(reader->input_file);
	reader->destroy = NULL; // now double deletion will cause segfault or assertion failure
	free(reader);
}

int reader_file_getc(ReaderFile *reader)
	{ return fgetc(reader->input_file); }

int reader_file_ungetc(ReaderFile *reader, int ch)
	{ return ungetc(ch, reader->input_file); }