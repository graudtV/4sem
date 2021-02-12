#include "writer_max_words.h"
#include "macro.h"
#include <stdlib.h> // for qsort()

static void writer_max_words_run_by_base_ptr(Writer *writer, HashTable *htbl, FILE *output_file)
	{ writer_max_words_run((WriterMaxWords *) writer, htbl, output_file); }

/* HashTableEntry with more words is less (!) than one with less words
 * (i.e. stands earlier in sorted array) */
static int max_words_cmp(const void *arg1, const void *arg2)
{
	const HashTableEntry *fst = *(const HashTableEntry **) arg1;
	const HashTableEntry *snd = *(const HashTableEntry **) arg2;
	
	/* using return (int) snd->count - (int) fst->count; is not safe for big numbers */
	if (snd->count == fst->count)
		return 0;
	return (snd->count > fst->count) ? 1 : -1;
}

WriterMaxWords *writer_max_words_create()
{
	ALLOC(WriterMaxWords, writer);
	writer->run = writer_max_words_run_by_base_ptr;
	return writer;
}

void writer_max_words_run(WriterMaxWords *writer, HashTable *htbl, FILE *output_file)
{
	size_t i = 0;
	size_t nentries = hashtable_size(htbl);
	const HashTableEntry *entries[nentries];

	for (HashTableIterator it = hashtable_begin(htbl);
		!hashtable_iterator_end_reached(&it);
		hashtable_iterator_inc(&it)) {

		entries[i++] = hashtable_iterator_get(&it);
	}
	qsort(entries, nentries, sizeof entries[0], max_words_cmp);
	for (i = 0; i < nentries; ++i)
		printf("%35s: %zu\n", entries[i]->str, entries[i]->count);
}