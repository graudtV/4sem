#ifndef PARSER_H_
#define PARSER_H_

#include "reader.h"
#include <stdlib.h>
#include <stdio.h> // for EOF

typedef enum {
	PARSER_SUCCESS = 0,
	PARSER_EOF = EOF,
	PARSER_NOT_ENOUGH_MEMORY
} ParserResult;

/* Interface for converting bytes from reader to words */
typedef struct Parser_ {
/* private: */
	Reader *reader;
	ParserResult (*get_word)(struct Parser_ *parser, char *buf, size_t bufsz);
} Parser;

/* macros are prefered to calling methods directly  */
#define parser_get_word(parser, buf, bufsz) ((parser)->get_word((Parser *) parser, (buf), (bufsz)))
#define parser_destroy(parser) free(parser)

#endif // PARSER_H_