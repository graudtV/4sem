#ifndef WRITER_MAX_WORDS_H_
#define WRITER_MAX_WORDS_H_

#include "writer.h"
#include "oop_tags.h"

/* Writes words from most frequent to less ones */
typedef struct {
	INHERITS Writer;
} WriterMaxWords;

WriterMaxWords *writer_max_words_create();
void writer_max_words_run(WriterMaxWords *writer, HashTable *htbl, FILE *output_file);

#endif // WRITER_MAX_WORDS_H_