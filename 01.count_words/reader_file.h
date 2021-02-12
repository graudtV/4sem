#ifndef READER_FILE_H
#define READER_FILE_H

#include "reader.h"
#include "oop_tags.h"
#include <stdio.h>

typedef struct {
	INHERITS Reader;
/* private: */
	FILE *input_file;
} ReaderFile;

ReaderFile *reader_file_create(const char *filename);
ReaderFile *reader_file_create2(FILE *input_file);
void reader_file_destroy(ReaderFile *reader);
int reader_file_getc(ReaderFile *reader);
int reader_file_ungetc(ReaderFile *reader, int ch);

#endif // READER_FILE_H