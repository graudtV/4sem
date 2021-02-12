#include "reader_cmd.h"
#include "macro.h"
#include <stdlib.h>
#include <stdio.h> // for EOF
#include <errno.h>
#include <assert.h>

static void reader_cmd_destroy_by_base_ptr(Reader *reader)
	{ reader_cmd_destroy((ReaderCmd *) reader); }

static int reader_cmd_getc_by_base_ptr(Reader *reader)
	{ return reader_cmd_getc((ReaderCmd *) reader); }

static int reader_cmd_ungetc_by_base_ptr(Reader *reader, int ch)
	{ return reader_cmd_ungetc((ReaderCmd *) reader, ch); }

/*  All elements in argv will be taken into account. If argv[0] (programm name)
 * should be ommitted, use
 * ReaderCmd *reader = reader_cmd_create(argc - 1, argv + 1); */
ReaderCmd *reader_cmd_create(int argc, char * const argv[])
{
	assert(argc >= 0);
	assert(argv != NULL || argc == 0);

	ALLOC(ReaderCmd, reader);
	reader->getc = reader_cmd_getc_by_base_ptr;
	reader->ungetc = reader_cmd_ungetc_by_base_ptr;
	reader->destroy = reader_cmd_destroy_by_base_ptr;
	reader->argc = argc;
	reader->argv = argv;
	reader->current_pos = (argc > 0) ? argv[0] : NULL;
	reader->ungetc_buf = EOF; // EOF = empty
	return reader;
}

void reader_cmd_destroy(ReaderCmd *reader)
{
	if (!reader)
		return;
	assert(reader->destroy != NULL);
	assert((void *) reader->destroy == (void *) reader_cmd_destroy_by_base_ptr
		&& "reader_cmd_destroy() invoked for not ReaderCmd object");
	reader->destroy = NULL; // now double deletion will cause segfault or assertion failure
	free(reader);
}

int reader_cmd_getc(ReaderCmd *reader)
{
	int c;
	if ((c = reader->ungetc_buf) != EOF) { // check if there is smth in buffer
		reader->ungetc_buf = EOF;
		return c;
	}
	if (!reader->current_pos)
		return EOF;
	if (!*reader->current_pos) { // end of argument reached
		if (reader->argc > 0) {
			reader->current_pos = *++reader->argv;
			--reader->argc;
		} else
			reader->current_pos = NULL;		
		return ' ';
	}
	return *(reader->current_pos++);
}

int reader_cmd_ungetc(ReaderCmd *reader, int ch)
{
	if (ch == EOF || reader->ungetc_buf != EOF)
		return EOF;
	return reader->ungetc_buf = ch;
}