#ifndef WRITER_H_
#define WRITER_H_

#include "hashtable.h"
#include <stdio.h> // for FILE

typedef struct Writer_ {
	void (*run) (struct Writer_ *writer, HashTable *htbl, FILE *output_file);
} Writer;

/* macros are prefered to calling methods directly  */
#define writer_run(writer, htbl, output_file) ((writer)->run((Writer *) (writer), (htbl), (output_file)))
#define writer_destroy(writer) free((writer))

#endif // WRITER_H_