//parse.c
#include "parse.h"
#include "ctype.h"
#include "string.h"
#include <stdio.h>
// Tokenize the input of the user
// Returns the number of tokens if there is no error, or -1 if there is an error
int tokenize(const char* input, char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN])
{
    int input_len = strlen(input);
    if (input_len == 0) {
        // No tokens
        return 0;
    }

    int token_i = 0;
    int token_len = 0;
    for (int i = 0; i < input_len; ++i) {
        if (isspace(input[i])) {
            if (token_len == 0) {
                // Skip
            } else {
                if (token_len >= MAX_TOKEN_LEN) {
                    return -1;
                }

                // Finalize current token
                tokens[token_i][token_len] = '\0';

                // Go to next token to scan
                token_len = 0;
                token_i++;
            }
        } else {
            if (token_i >= MAX_NB_TOKENS) {
                return -1;
            }

            // Add character to current token
            tokens[token_i][token_len] = input[i];
            token_len++;
        }
    }

    if (token_len > 0) {
        if (token_len >= MAX_TOKEN_LEN) {
            return -1;
        }

        // Finalize last token
        tokens[token_i][token_len] = '\0';
        token_i++;
    }

    // Number of tokens
    return token_i;
}

// Parse tokens into CommandSequence
// Returns the number of consumed tokens, if there is no error, or -1 if there
// is an error It calls the other related parse_* functions
int parse(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    CommandSequence* command)
{
    // total consumed
    int cursor = 0;

    // Initially no commands
    command->nb_cmds = 0;

    bool sequential = false;
    bool parallel = false;

    // While there is remaining tokens
    while (nb_tokens - cursor) {
        if (command->nb_cmds >= MAX_NB_CMDS) {
            return -1;
        }

        // Try to parse a compound command
        int consumed = parse_compound_command(
            &tokens[cursor], nb_tokens - cursor, &command->cmds[command->nb_cmds]);

        if (consumed == -1) {
            return -1;
        }

        command->nb_cmds++;
        cursor += consumed;

        if (nb_tokens - cursor == 0) {
            break;
        }

        // Try parse ; or &
        if (strcmp(tokens[cursor], ";") == 0) {
            if (parallel) {
                // previously marked as parallel

                // Cannot mix ; and &
                return -1;
            }

            // Mark as sequential
            sequential = true;
            cursor++;
        } else if (strcmp(tokens[cursor], "&") == 0) {
            if (sequential) {
                // previously marked as sequential

                // Cannot mix ; and &
                return -1;
            }

            // mark as parallel
            parallel = true;
            cursor++;
        } else {
            return -1;
        }
    }

    command->in_parallel = parallel;

    return nb_tokens;
}

// Parse tokens into CompoundCommand
// Returns the number of consumed tokens, if there is no error, or -1 if there
// is an error It calls the other related parse_* functions Take not that if
// there is one SimpleCommand, the CompoundCommand must have op = OPERATOR_NONE
// which means that cmd2 is not filled.
int parse_compound_command(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    CompoundCommand* command)
{
    int consumed = parse_simple_command(
        tokens, nb_tokens, &command->cmd1);

    if (consumed == -1) {
        return -1;
    }

    if (strcmp(tokens[consumed], "&&") == 0) {
        command->op = OPERATOR_AND;
        consumed++;
    } else if (strcmp(tokens[consumed], "||") == 0) {
        command->op = OPERATOR_OR;
        consumed++;
     } else if(strcmp(tokens[consumed],"|")== 0){
	command->op = OPERATOR_PIPE;
	consumed++;

    } else {
        command->op = OPERATOR_NONE;
        return consumed;
    }

    int consumed_cmd2 = parse_simple_command(
        &tokens[consumed], nb_tokens - consumed, &command->cmd2);

    if (consumed_cmd2 == -1) {
        return -1;
    }

    return consumed + consumed_cmd2;
}

// Parse tokens into SimpleCommand
// Returns the number of consumed tokens, if there is no error, or -1 if there
// is an error
int parse_simple_command(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    SimpleCommand* command)
{
    if (nb_tokens < 1 || 
        strcmp(tokens[0], "&&") == 0 || 
        strcmp(tokens[0], "||") == 0 || 
        strcmp(tokens[0], "|") == 0 || 
        strcmp(tokens[0], ";") == 0 || 
        strcmp(tokens[0], "&") == 0) {
        return -1;
    }

    strcpy(command->program_name, tokens[0]);
    command->nb_args = 0;
    command->output_file[0] = '\0'; // Initialize the output_file field

    for (int i = 1; i < nb_tokens; ++i) {
        if (strcmp(tokens[i], ">") == 0) {
            if (i + 1 < nb_tokens) {
                strcpy(command->output_file, tokens[i + 1]);
                command->nb_args = i - 1; // Set the number of arguments excluding redirection
                return i + 2; // Return number of tokens processed
            } else {
                fprintf(stderr, "Error: No output file specified for redirection\n");
                return -1;
            }
        }

        if (command->nb_args < MAX_NB_ARGS) {
            strcpy(command->args[command->nb_args], tokens[i]);
            command->nb_args++;
        } else {
            fprintf(stderr, "Error: Too many arguments\n");
            return -1;
        }
    }

    return nb_tokens; // Return the number of tokens processed if no redirection
}
