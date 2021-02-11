#ifndef MACRO_H_
#define MACRO_H_

#include <stdlib.h>
#include <errno.h>

/* creates varname dynamically */
#define ALLOC(Type, varname)								\
	Type *(varname);										\
	if (((varname) = malloc(sizeof(Type))) == NULL) {		\
		errno = ENOMEM;										\
		return NULL;										\
	}

#endif // MACRO_H_