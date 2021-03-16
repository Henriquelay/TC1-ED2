/**
 * This library is for manipulating inputs as decribed on the specification
 * */
#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE* openFile(const char* fileName, const char* mode);
char closeFile(FILE* file);

char** readLine(FILE* file, const char *separator, char* buffer, size_t* bufferSize, const size_t* nFeatures);
size_t getLineSize(FILE* file, const char separator, char* buffer, size_t* bufferSize);
size_t countLines(FILE* file);

#endif
