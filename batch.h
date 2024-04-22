// batch.h

#ifndef BATCH_H
#define BATCH_H

#include "parse.h"
#include "launch.h"

// Run the Shell in batch mode
// Reads a file and executes commands line by line
// Returns 0 if there is no error, or -1 if there is an error
int batch_mode(const char* filename);

#endif // BATCH_H
