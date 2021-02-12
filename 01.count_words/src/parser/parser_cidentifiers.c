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

static int isfstletter(int c) { return isalpha(c) || c == '_'; }
static int issndletter(int c) { return isalnum(c) || c == '_'; }

ParserResult parser_cidentifiers_get_word(ParserCIdentifiers *parser, char *buf, size_t bufsz)
{
	assert(parser != NULL);
	assert(buf != NULL);
	
	int c;
	while ((c = reader_getc(parser->reader)) != EOF && !isfstletter(c))
		;
	if (c == EOF)
		return PARSER_EOF;
	while (bufsz > 1) {
		*buf++ = c;
		--bufsz;
		c = reader_getc(parser->reader);
		if (!issndletter(c))
			break;
	}
	*buf = '\0';
	if (bufsz > 1 || !issndletter(c)) // second cond - word precisely fits buffer
		return PARSER_SUCCESS;
	reader_ungetc(parser->reader, c);
	return PARSER_NOT_ENOUGH_MEMORY;
}