#include "../lib/file.h"

/**
 * Opens file as readonly, checks for errors, exit if any errors, then returns
 * the new file.
 * */
FILE* openFile(const char* fileName, const char* mode) {
    FILE* newFile = fopen(fileName, mode);
    if (newFile == NULL) {
        perror("Error opening file. Exiting");
        exit(EXIT_FAILURE);
    }
    return newFile;
}

/**
 * Closes an open file
 * */
char closeFile(FILE* file) {
    if (file == NULL) {
        return 0;
    }
    return fclose(file);
}


// TODO usar o getline espertamente para não precisar de getLineSize e countLines
/**
 * Returns how many features are in a like of input, not including the identifier.
 * Takes a buffer as input to avoid keep re-allocating memory
 * */
size_t getLineSize(FILE* file, const char separator, char* buffer, size_t* bufferSize) {
    fpos_t previousPosition;   // Stores previous position
    fgetpos(file, &previousPosition);

    size_t charsRead = 0;
    if ((charsRead = getline(&buffer, bufferSize, file)) == -1) {
        perror("Error getting new line. Exiting");
        exit(EXIT_FAILURE);
    }
    fsetpos(file, &previousPosition);  // Restores previous position

    size_t counter = 0;
    for (size_t i = 0; i < charsRead; i++) {
        if (buffer[i] == separator) {
            counter++;
        }
    }
    return counter;
}

/**
 * Counts and returns the number of lines in the file
 * */
size_t countLines(FILE* file) {
    fpos_t previousPosition;   // Stores previous position
    fgetpos(file, &previousPosition);

    size_t lines = 0;
    for (char ch = fgetc(file); !feof(file); ch = fgetc(file))
        if (ch == '\n') lines++;

    fsetpos(file, &previousPosition);  // Restores previous position
    return lines;
}

/**
 * Takes an open file, returns an allocated struct with identifier and values
 * Takes a buffer as input to avoid keep re-allocating memory
 * */
char** readLine(FILE* file, const char* separator, char* buffer, size_t* bufferSize, const size_t* nFeatures) {
    if (getline(&buffer, bufferSize, file) == -1) {
        return NULL;
    }

    char** tokens = (char**)malloc(sizeof(char*) * (*nFeatures + 1));
    if (tokens == NULL) {
        perror("Error allocating tokens array");
        exit(EXIT_FAILURE);
    }

    // Alternativelly, a null pointer may be specified, in which case the function
    //continues scanning where a previous successful call to the function ended.
    char* token = strtok(buffer, separator);
    // NOTE - Try removing one of the checks for's second term to gain a bit of time if it works
    for (size_t i = 0; token != NULL; token = strtok(NULL, separator), i++) {
        // Allocates space for data cell
        tokens[i] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
        if (tokens[i] == NULL) {
            perror("Error allocating feature for dataPoint. Exiting");
            exit(EXIT_FAILURE);
        }
        strcpy(tokens[i], token);
    }
    return tokens;
}
