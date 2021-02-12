SOURCES=\
 src/main.c\
 src/reader/reader_file.c\
 src/reader/reader_cmd.c\
 src/parser/parser_cidentifiers.c\
 src/hashtable/hashtable.c\
 src/writer/writer_max_words.c

INCLUDE_PATHS=\
 include/hashtable\
 include/other\
 include/parser\
 include/reader\
 include/writer

HEADERS=$(addsuffix /*,$(INCLUDE_PATHS))

EXTRA_FLAGS=-fms-extensions -Wno-microsoft

all: bin/count-words

run: bin/count-words
	./bin/count-words

bin/count-words: $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(EXTRA_FLAGS) $(addprefix -I ,$(INCLUDE_PATHS)) $(CFLAGS) -o $@

clean:
	rm -rf bin/count-words
