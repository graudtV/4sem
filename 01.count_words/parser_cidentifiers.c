#include "parser_cidentifiers.h"
#include "macro.h"
#include <assert.h>
#include <ctype.h>

static ParserResult parser_cidentifiers_get_word_by_base_ptr(Parser *parser, char *buf, size_t bufsz)
	{ return parser_cidentifiers_get_word((ParserCIdentifiers *) parser, buf, bufsz); }

ParserCIdentifiers *parser_cidentifiers_create(Reader *reader)
{
	ALLOC(ParserCIdentifiers, parser);
	parser->reader = reader;
	parser->get_word = parser_cidentifiers_get_word_by_base_ptr;
	return parser;
}

ParserResult parser_cidentifiers_get_word(ParserCIdentifiers *parser, char *buf, size_t bufsz)
{
	assert(parser != NULL);
	assert(buf != NULL);
	
	int c;
	while ((c = reader_getc(parser->reader)) != EOF && !isalpha(c) && c != '_')
		;
	if (c == EOF)
		return PARSER_EOF;
	while (bufsz > 1) { // 1 - for '\0'
		*buf++ = c;
		--bufsz;
		c = reader_getc(parser->reader);
		if (!isalnum(c) && c != '_')
			break;
	}
	reader_ungetc(parser->reader, c);
	*buf = '\0';
	return (bufsz > 1) ? PARSER_SUCCESS : PARSER_NOT_ENOUGH_MEMORY;
}