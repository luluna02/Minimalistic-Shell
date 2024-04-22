//parse.h
#ifndef __H_PARSE__
#define __H_PARSE__

#include <stdbool.h>

#define MAX_NB_TOKENS 1000
#define MAX_TOKEN_LEN 50
#define MAX_NB_ARGS 10
#define MAX_NB_CMDS 10

struct SimpleCommand {
    char program_name[MAX_TOKEN_LEN];
    char args[MAX_NB_ARGS][MAX_TOKEN_LEN];
    int nb_args;
    char output_file[MAX_TOKEN_LEN];
};
typedef struct SimpleCommand SimpleCommand;

enum CommandOperator {
    OPERATOR_NONE,
    OPERATOR_AND,
    OPERATOR_OR,
    OPERATOR_PIPE
};
typedef enum CommandOperator CommandOperator;

struct CompoundCommand {
    SimpleCommand cmd1;
    SimpleCommand cmd2;
    CommandOperator op;
};
typedef struct CompoundCommand CompoundCommand;

struct CommandSequence {
    CompoundCommand cmds[MAX_NB_CMDS];
    int nb_cmds;
    bool in_parallel;
};
typedef struct CommandSequence CommandSequence;

// Tokenize the input of the user
// Returns the number of tokens if there is no error, or -1 if there is an error
int tokenize(const char* input, char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN]);

// Parse tokens into CommandSequence
// Returns the number of consumed tokens, if there is no error, or -1 if there
// is an error It calls the other related parse_* functions
int parse(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    CommandSequence* command);

// Parse tokens into CompoundCommand
// Returns the number of consumed tokens, if there is no error, or -1 if there
// is an error It calls the other related parse_* functions Take not that if
// there is one SimpleCommand, the CompoundCommand must have op = OPERATOR_NONE
// which means that cmd2 is not filled.
int parse_compound_command(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    CompoundCommand* command);

// Parse tokens into SimpleCommand
// Returns the number of consumed tokens, if there is no error, or -1 if there
// is an error
int parse_simple_command(
    char tokens[MAX_NB_TOKENS][MAX_TOKEN_LEN],
    int nb_tokens,
    SimpleCommand* command);

#endif // __H_PARSE__

