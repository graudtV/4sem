#ifndef READER_H_
#define READER_H_

typedef struct Reader_ {
/* private: */
	int (*getc) (struct Reader_ *); // returns character or EOF when finished
	int (*ungetc) (struct Reader_ *, int ch); // same as ungetc is stdio.h
	void (*destroy) (struct Reader_ *r);
} Reader;

/* macros are prefered to calling methods directly  */
#define reader_getc(reader) ((reader)->getc((Reader *) reader))
#define reader_ungetc(reader, ch) ((reader)->ungetc((Reader *) reader, ch))
#define reader_destroy(reader) ({ if (reader) (reader)->destroy((Reader *) reader); })

#endif // READER_H_