// batch.c

#include "batch.h"
#include "parse.h"  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT_LENGTH 1024 


int batch_mode(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char input_line[MAX_INPUT_LENGTH];
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN];
    CommandSequence command_sequence;

    while (fgets(input_line, MAX_INPUT_LENGTH, file) != NULL) {
        input_line[strcspn(input_line, "\n")] = 0; // Remove newline

        int nb_tokens = tokenize(input_line, tokens); // Tokenize the input
        if (nb_tokens == -1) {
            fprintf(stderr, "Error: Failed to tokenize input\n");
            continue;
        }

        if (parse(tokens, nb_tokens, &command_sequence) == -1) {
            fprintf(stderr, "Error: Failed to parse command\n");
            continue;
        }

        if (launch_command(&command_sequence) == -1) {
            fprintf(stderr, "Error: Failed to execute command\n");
        }
    }

    fclose(file);
    return 0;
}
