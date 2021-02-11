#include "reader.h"
#include "reader_cmd.h"
#include "reader_file.h"
#include "parser.h"
#include "parser_cidentifiers.h"
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[])
{
	Reader *reader_file = (Reader *) reader_file_create("main.c");
	Reader *reader_cmd = (Reader *) reader_cmd_create(argc - 1, argv + 1);
	Reader *reader = reader_file;

	Parser *parser = (Parser *) parser_cidentifiers_create(reader);

	if (!reader)
		perror("error");

	char buf[1000];
	int err;
	while ((err = parser_get_word(parser, buf, sizeof buf)) == PARSER_SUCCESS)
		printf("word: '%s'\n", buf);

	reader_destroy(reader_file);
	reader_destroy(reader_cmd);
	return 0;
}