#include "strings.h"


Strings_t *init_strings() {
    Strings_t *strings = malloc(sizeof(Strings_t));

    strings->pointers = malloc(sizeof(size_t) * STRING_MAX);
    strings->chunks = malloc(sizeof(size_t) * STRING_MAX);
    strings->data = malloc(sizeof(int *) * STRING_MAX);
    strings->string = 0;

    for (int i = 0; i < STRING_MAX; i++) {
        strings->pointers[i] = 0;
        strings->chunks[i] = 1;
        strings->data[i] = malloc(sizeof(int) * CHUNK_SIZE);
        for (int j = 0; j < CHUNK_SIZE; j++) {
            strings->data[i][j] = 0;
        }
    }

    return strings;
}

void grow_string(Strings_t *string, unsigned char i) {
    if (i >= STRING_MAX) {
        fprintf(stderr, "Invalid string index\n");
        exit(1);
    }
    string->chunks[i]++;
    string->data[i] = realloc(string->data[i], sizeof(int) * CHUNK_SIZE * string->chunks[i]);
}

void shrink_string(Strings_t *string, unsigned char i) {
    if (i >= STRING_MAX) {
        fprintf(stderr, "Invalid string index\n");
        exit(1);
    }
    string->chunks[i]--;
    if (string->chunks[i] == 0) {
        fprintf(stderr, "String size cannot be 0\n");
        exit(1);
    }
    string->data[i] = realloc(string->data[i], sizeof(int) * CHUNK_SIZE * string->chunks[i]);
}

void next_string(Strings_t *string) {
    if (string->string == STRING_MAX - 1) {
        fprintf(stderr, "Invalid string index\n");
        exit(1);
    }
    string->string++;
}

void prev_string(Strings_t *string) {
    if (string->string == 0) {
        fprintf(stderr, "Invalid string index\n");
        exit(1);
    }
    string->string--;
}

void goto_string(Strings_t *string, unsigned char index) {
    if (index < 0 || index >= STRING_MAX) {
        fprintf(stderr, "Invalid string index\n");
        exit(1);
    }
    string->string = index;
}

void next_cell(Strings_t *string) {
    size_t i = string->string;
    if ((string->chunks[i] + 1) % CHUNK_SIZE == 0) {
        grow_string(string, i);
    }
    string->pointers[i]++;
}

void prev_cell(Strings_t *string) {
    size_t i = string->string;
    if (string->pointers[i] == 0) {
        fprintf(stderr, "Invalid cell index\n");
        exit(1);
    }
    if (string->chunks[i] % CHUNK_SIZE == 0) {
        shrink_string(string, i);
    }
    string->pointers[string->string]--;
}

void inc_cell(Strings_t *string) {
    size_t i = string->string;
    string->data[i][string->pointers[i]]++;
}

void dec_cell(Strings_t *string) {
    size_t i = string->string;
    string->data[i][string->pointers[i]]--;
}

int read_cell(Strings_t *string) {
    size_t i = string->string;
    return string->data[i][string->pointers[i]];
}

bool is_valid_char(char c) {
    return (
            c == '+' ||
            c == '-' ||
            c == '>' ||
            c == '<' ||
            c == '[' ||
            c == ']' ||
            c == '.' ||
            c == ',' ||
            (c >= '0' && c <= '9') ||
            c == 'A' || c == 'a' ||
            c == 'B' || c == 'b' ||
            c == 'C' || c == 'c' ||
            c == 'D' || c == 'd' ||
            c == 'E' || c == 'e' ||
            c == 'F' || c == 'f' ||
            c == '`' ||
            c == '!'
    );
}

void process_source(Strings_t *string, unsigned char *source, size_t length) {
    size_t opens_length = 0;
    size_t *opens = malloc(sizeof(size_t) * opens_length);
}