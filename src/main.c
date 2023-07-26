#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr, "Source file path is required\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        fprintf(stderr, "Cannot open source file\n");
        exit(1);
    }

    char c;

    while((c = fgetc(file)) != EOF){
        fprintf(stdout, "%c\n", c);
    }

    fclose(file);
    return 0;
}