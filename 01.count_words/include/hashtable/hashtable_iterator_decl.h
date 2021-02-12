/* hashtable_iterator_decl.h: declaration of HashTableIterator type */
#ifndef HASHTABLE_ITERATOR_DECL_H_
#define HASHTABLE_ITERATOR_DECL_H_

#include <stdlib.h> // for size_t

/*  This structure has to be visible to use HashTable iterators.
 *  Nevertheless, accessing it directly from client code may cause
 * undefined behaviour */
typedef struct HashTableIterator_ {
/* private: */
	struct HashTableNode_ **main_list_pos; // NULL for end iterator
	struct HashTableNode_ *side_list_pos;
	size_t nodes_left; // in main list. Decremented when hashtable side list fully passed
} HashTableIterator;

#endif // HASHTABLE_ITERATOR_DEC_H_