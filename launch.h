//Launch.h
#pragma once
#include "parse.h"

// Execute a simple or compound command
// Returns 0 if there is no error, or -1 if there is an error
// It calls the other related launch_* functions
int launch_command(CommandSequence* command);
// Execute a compound command
// Returns 0 if there is no error, or -1 if there is an error
// It calls the other related launch_* functions
int launch_compound_command(CompoundCommand* command);
// Execute a simple command
// Returns 0 if there is no error, or -1 if there is an error
int launch_simple_command(SimpleCommand* command,int read_pipe,int write_pipe);
