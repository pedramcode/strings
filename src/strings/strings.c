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
        fprintf(stderr, "Cannot grow string(%d)\n", i);
        exit(1);
    }
    string->chunks[i]++;
    string->data[i] = realloc(string->data[i], sizeof(int) * CHUNK_SIZE * string->chunks[i]);
}

void shrink_string(Strings_t *string, unsigned char i) {
    if (i >= STRING_MAX) {
        fprintf(stderr, "Cannot shrink string(%d)\n", i);
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

void set_cell(Strings_t *string, unsigned char c) {
    string->data[string->string][string->pointers[string->string]] = c;
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
            c == '!' ||
            c == '\n'
    );
}

int char_to_num(unsigned char c) {
    if (c >= '0' && c <= '9') {
        return c - 48;
    }
    if (c == 'A' || c == 'a') return 10;
    if (c == 'B' || c == 'b') return 11;
    if (c == 'C' || c == 'c') return 12;
    if (c == 'D' || c == 'd') return 13;
    if (c == 'E' || c == 'e') return 14;
    if (c == 'F' || c == 'f') return 15;
    fprintf(stderr, "Invalid string symbol\n");
    exit(1);
}

typedef enum {
    OPEN = 1,
    CLOSE = 2,
} CondType;

typedef struct {
    CondType _type;
    size_t index;
} CondInfo_t;

void process_source(Strings_t *string, const unsigned char *source, size_t length) {
    size_t index = 0;

    Stack_t *stack = stack_new();

    bool comment_mode = false;
    bool skip_mode = false;
    size_t skip_index = 0;

    size_t line = 1;
    size_t col = 0;

    while (true) {
        col++;
        unsigned char c = source[index];
        switch (c) {
            case '+':
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                inc_cell(string);
                index++;
                break;
            case '-':
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                dec_cell(string);
                index++;
                break;
            case '>':
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                next_cell(string);
                index++;
                break;
            case '<':
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                prev_cell(string);
                index++;
                break;
            case '[': {
                CondInfo_t *info = malloc(sizeof(CondInfo_t));
                info->_type = OPEN;
                info->index = index;
                stack_push(stack, info);
                if (read_cell(string) == 0) {
                    skip_mode = true;
                    skip_index = index;
                }
                index++;
                break;
            }
            case ']': {
                if (skip_mode && ((CondInfo_t *) stack_peek(stack)) != NULL &&
                    ((CondInfo_t *) stack_peek(stack))->index == skip_index) {
                    skip_mode = false;
                } else if (((CondInfo_t *) stack_peek(stack)) == NULL) {
                    fprintf(stderr, "Invalid control symbol\n");
                    exit(1);
                } else if (read_cell(string) != 0) {
                    index = ((CondInfo_t *) stack_peek(stack))->index + 1;
                } else {
                    free(stack_pop(stack));
                    index++;
                }

                break;
            }
            case '\n':
                comment_mode = false;
                line++;
                index++;
                col = 1;
                break;
            case ';':
                comment_mode = true;
                index++;
                break;
            case '.':
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                fprintf(stdout, "%c", read_cell(string));
                index++;
                break;
            case ',': {
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                unsigned char ic;
                fscanf(stdin, "%c", &ic);
                set_cell(string, ic);
                index++;
                break;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'a':
            case 'B':
            case 'b':
            case 'C':
            case 'c':
            case 'D':
            case 'd':
            case 'E':
            case 'e':
            case 'F':
            case 'f': {
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                int n = char_to_num(c);
                goto_string(string, n);
                index++;
                break;
            }
            case '`':
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                next_string(string);
                index++;
                break;
            case '!':
                if (comment_mode || skip_mode) {
                    index++;
                    break;
                }
                prev_string(string);
                index++;
                break;
            case '\t':
            case ' ':
                index++;
                break;
            default:
                if (!comment_mode) {
                    fprintf(stderr, "Invalid symbol %ld:%ld\n", line, col);
                    exit(1);
                }
                index++;
                break;
        }
        if (index == length)break;
    }
}