#ifndef READER_CMD_H
#define READER_CMD_H

#include "reader.h"
#include "oop_tags.h"

typedef struct {
	INHERITS Reader;
/* private: */
	int argc;
	char * const * argv;
	const char *current_pos;
	char ungetc_buf;
} ReaderCmd;

ReaderCmd *reader_cmd_create(int argc, char * const argv[]);
void reader_cmd_destroy(ReaderCmd *reader);
int reader_cmd_getc(ReaderCmd *reader);
int reader_cmd_ungetc(ReaderCmd *reader, int ch);

#endif // READER_CMD_H