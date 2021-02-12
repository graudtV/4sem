/* hashtable.h: Unordered multiset for C-strings implementation */

#include "hashtable.h"
#include "macro.h"
#include <assert.h>
#include <string.h>

typedef struct HashTableNode_ {
	HashTableEntry entry;
	struct HashTableNode_ *next;
} HashTableNode;

typedef struct HashTable_ {
	HashTableNode **nodes;
	size_t nodes_sz; // size of nodes array
	size_t nelems; // totally in hashtab
} HashTable;

static size_t hash(const char *str)
{
	return 0;//strlen(str);
}

/*  init_sz must be > 0
 *  init_sz is used as number of lists. Number of elems in hashtable can exceed it */
HashTable *hashtable_create(size_t init_sz)
{
	assert(init_sz > 0);
	ALLOC(HashTable, htbl);
	if ((htbl->nodes = calloc(init_sz, sizeof(HashTableNode *))) == NULL) {
		free(htbl);
		errno = ENOMEM;
		return NULL;
	}
	htbl->nodes_sz = init_sz;
	htbl->nelems = 0;
	return htbl;
}

void hashtable_destroy(HashTable *htbl)
{
	if (!htbl)
		return;
	for (HashTableNode **p = htbl->nodes; p < htbl->nodes + htbl->nodes_sz; ++p)
		for (HashTableNode *q = *p; q != NULL; q = q->next)
			free((void *) q->entry.str);
	free(htbl);
}

#include <stdio.h>
const HashTableEntry *hashtable_insert(HashTable *htbl, const char *str)
{
	assert(htbl->nodes_sz > 0);
	size_t idx = hash(str) % htbl->nodes_sz;
	HashTableNode **ptr_to_last = &htbl->nodes[idx]; // pointer to the place, where to write link for a new node, if it will be created

	/* seek equal str or end of list */
	for (HashTableNode *p = *ptr_to_last; p != NULL; p = p->next) {
		if (strcmp(p->entry.str, str) == 0) { // found
			++p->entry.count;
			return &p->entry;
		}
		ptr_to_last = &p->next;
	}
	ALLOC(HashTableNode, new_node);
	if ((new_node->entry.str = strdup(str)) == NULL) {
		free(new_node);
		errno = ENOMEM;
		return NULL;
	}
	new_node->next = NULL;
	new_node->entry.count = 1;
	*ptr_to_last = new_node; // connect node to the hashtable
	++htbl->nelems;
	return &new_node->entry;
}

size_t hashtable_size(HashTable *htbl)
	{ return htbl->nelems; }

HashTableIterator hashtable_begin(HashTable *htbl)
{
	HashTableIterator it;
	it.main_list_pos = htbl->nodes;
	it.side_list_pos = NULL;
	it.nodes_left = htbl->nodes_sz;

	assert(it.main_list_pos != NULL); // never NULL because htbl->nodes cannot have size == 0
	while (*it.main_list_pos == NULL && it.nodes_left > 0) // seeking for not NULL in nodes array
		++it.main_list_pos, --it.nodes_left;
	if (!it.nodes_left) // hashtable is empty
		it.main_list_pos = NULL;
	else // side list found
		it.side_list_pos = *it.main_list_pos;
	return it;
}

bool hashtable_iterator_end_reached(HashTableIterator *it)
	{ return it->main_list_pos == NULL; }

void hashtable_iterator_inc(HashTableIterator *it)
{
	assert(!hashtable_iterator_end_reached(it));
	if ((it->side_list_pos = it->side_list_pos->next) != NULL) // elem in side list exists
		return;
	/* seeking for the next not-empty side list */
	while (--it->nodes_left > 0) {
		if (*++it->main_list_pos) { // list found
			it->side_list_pos = *it->main_list_pos;
			return;
		}
	}
	/* end of hashtable reached */
	it->main_list_pos = NULL;
}

const HashTableEntry *hashtable_iterator_get(HashTableIterator *it)
{
	assert(!hashtable_iterator_end_reached(it));
	return &(it->side_list_pos->entry);
}