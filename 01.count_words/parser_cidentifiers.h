#ifndef PARSER_CIDENTIFIERS_H_
#define PARSER_CIDENTIFIERS_H_

#include "parser.h"
#include "oop_tags.h"

typedef struct {
	INHERITS Parser;
} ParserCIdentifiers;

ParserCIdentifiers *parser_cidentifiers_create(Reader *reader);
ParserResult parser_cidentifiers_get_word(ParserCIdentifiers *parser, char *buf, size_t bufsz);

#endif // PARSER_CIDENTIFIERS_H_