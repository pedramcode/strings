#ifndef STRINGS_H
#define STRINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include "stack/stack.h"

#define STRING_MAX 16
#define CHUNK_SIZE 128

typedef struct {
    int **data;
    size_t *pointers;
    size_t *chunks;
    unsigned char string;
} Strings_t;

Strings_t *init_strings();

void process_source(Strings_t *string, const unsigned char* source, size_t length);

void grow_string(Strings_t *string, unsigned char i);
void shrink_string(Strings_t *string, unsigned char i);

void next_string(Strings_t *string);
void prev_string(Strings_t *string);
void goto_string(Strings_t *string, unsigned char index);

void next_cell(Strings_t *string);
void prev_cell(Strings_t *string);
void inc_cell(Strings_t *string);
void dec_cell(Strings_t *string);
int read_cell(Strings_t *string);
void set_cell(Strings_t *string, unsigned char c);

bool is_valid_char(char c);

#endif