#include <stdio.h>
#include <stdlib.h>

#include "strings/strings.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Source file path is required\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot read source file\n");
        exit(1);
    }

    Strings_t *strings = init_strings();
    const size_t read_chunk = 128;
    size_t chunks = 1;
    unsigned char *source = malloc(read_chunk);
    size_t source_index = 0;

    char c;
    while ((c = fgetc(file)) != EOF) {
        source[source_index++] = c;
        if (source_index % read_chunk == 0) {
            chunks++;
            source = realloc(source, read_chunk * chunks);
        }
    }
    fclose(file);

    process_source(strings, source, source_index);

    return 0;
}