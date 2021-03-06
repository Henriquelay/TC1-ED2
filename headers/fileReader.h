// Auxiliary to read inputs and file input stats.

#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE* openFile(const char* fileName);
char closeFile(FILE* file);

size_t getLineSize(FILE* file, const char* separator, char* buffer, size_t* bufferSize);
char** readLine(FILE* file, const char* separator, char* buffer, size_t* bufferSize, size_t* tokenAmount);

#endif
