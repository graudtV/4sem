#include "reader_cmd.h"
#include "reader_file.h"
#include "parser_cidentifiers.h"
#include "hashtable.h"
#include "writer_max_words.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[])
{
	Reader *reader = (argc > 1)
		? (Reader *) reader_cmd_create(argc - 1, argv + 1)
		: (Reader *) reader_file_create2(stdin);
	Parser *parser = (Parser *) parser_cidentifiers_create(reader);
	HashTable *htbl = hashtable_create(100);
	Writer *writer = (Writer *) writer_max_words_create();

	if (!reader || !parser || !htbl || !writer) {
		perror("error");
		exit(EXIT_FAILURE);
	}

	char buf[5];
	int err;
	while ((err = parser_get_word(parser, buf, sizeof buf)) == PARSER_SUCCESS)
		hashtable_insert(htbl, buf);
	if (err == PARSER_NOT_ENOUGH_MEMORY) {
		fprintf(stderr, "error: too long word '%s...' Max supported word length is %zu\n", buf, sizeof buf - 1); // 1 - for '\0'
		parser_get_word(parser, buf, sizeof buf);
		printf("%s\n", buf);

		exit(EXIT_FAILURE);
	}
	writer_run(writer, htbl, stdout);

	writer_destroy(writer);
	hashtable_destroy(htbl);
	parser_destroy(parser);
	reader_destroy(reader);
	return EXIT_SUCCESS;
}