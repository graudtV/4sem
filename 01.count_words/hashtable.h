#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdbool.h>
#include <stdlib.h> // for size_t

typedef struct HashTable_ HashTable;
typedef struct HashTableIterator_ *HashTableIterator;

typedef struct {
	const char *str;
	size_t count;
} HashTableEntry;

HashTable *hashtable_create(size_t init_sz);
void hashtable_destroy(HashTable *htbl);

/*  HashTable memory policy: inserted strs are automatically dublicated,
 * if they don't exist in hashtable yet, and automatically freed when
 * hashtable_destroy() is called */
const HashTableEntry *hashtable_insert(HashTable *htbl, const char *str);

/*  HashTableIterator memory policy: to avoid neccessity to deallocate
 * iterators (it is inconvinient and unsafe and will make imposible to make
 * C++-style for-loops with iterators), they are allocated from static storage
 *  Thus, no deallocation needed, but number of simultaneously created iterators
 * is limited
 */
HashTableIterator hashtable_begin(HashTable *htbl);
bool hashtable_iterator_end_reached(HashTableIterator it);
void hashtable_iterator_inc(HashTableIterator it);
const HashTableEntry *hashtable_iterator_get(HashTableIterator it);


#endif // HASHTABLE_H_