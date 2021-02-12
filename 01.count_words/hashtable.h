/* hashtable.h: Unordered multiset for C-strings (implemented on lists) */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "hashtable_iterator_decl.h"
#include <stdbool.h>

typedef struct HashTable_ HashTable;

typedef struct {
	const char *str;
	size_t count;
} HashTableEntry;

/*  HashTable memory policy:
 *  1) HashTable must be initialized with hashtable_create() or NULL and
 * destroyed with hashtable_destroy()
 *  2) insert(): inserted strs are automatically dublicated,
 * if they don't exist in hashtable yet, and automatically freed when
 * hashtable_destroy() is called */

/* HashTable interface: */
HashTable *hashtable_create(size_t init_sz);
void hashtable_destroy(HashTable *htbl);
const HashTableEntry *hashtable_insert(HashTable *htbl, const char *str);
size_t hashtable_size(HashTable *htbl); // number of different stored elements

/*  HashTableIterator memory policy: iterators must be initialized
 * with hashtable_begin(), no deallocation needed */

/* HashTableIterator interface */
HashTableIterator hashtable_begin(HashTable *htbl);
bool hashtable_iterator_end_reached(HashTableIterator *it);
void hashtable_iterator_inc(HashTableIterator *it);
const HashTableEntry *hashtable_iterator_get(HashTableIterator *it);


#endif // HASHTABLE_H_