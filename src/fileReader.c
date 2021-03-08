#include "../lib/fileReader.h"

/**
 * Opens file as readonly, checks for errors, exit if any errors, then returns
 * the new file.
 * */
FILE* openFile(const char* fileName) {
    FILE* newFile = fopen(fileName, "r");
    if (newFile == NULL) {
        perror("Error opening file. Exiting");
        exit(1);
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

/**
 * Returns how many tokens are in a like of input, including the identifier.
 * Takes a buffer as input to avoid keep re-allocating memory
 * */
size_t getLineSize(FILE* file, const char* separator, char* buffer, size_t* bufferSize) {
    size_t charsRead = 0;
    fpos_t previousPosition;   // Stores previous position
    fgetpos(file, &previousPosition);
    if ((charsRead = getline(&buffer, bufferSize, file)) == -1) {
        perror("Error getting new line. Exiting");
        exit(1);
    }
    fsetpos(file, &previousPosition);  // Restores previous position

    size_t counter = 0;
    for (int i = 0; i < charsRead; i++) {
        if (buffer[i] == *separator) {
            counter++;
        }
    }
    return ++counter;
}

/**
 * Counts and returns the number of lines in the file
 * */
size_t countLines(FILE* file) {
    fpos_t previousPosition;   // Stores previous position
    fgetpos(file, &previousPosition);

    size_t lines = 0;
    for (char ch = fgetc(file); ch != EOF; ch = fgetc(file)) {
        if (ch == '\n') {
            lines++;
        }
    }

    fsetpos(file, &previousPosition);  // Restores previous position
    return lines;
}

/**
 * Takes an open file, returns an allocated struct with identifier and values
 * Takes a buffer as input to avoid keep re-allocating memory
 * */
char** readLine(FILE* file, const char* separator, char* buffer, size_t* bufferSize, size_t* tokenAmount) {
    if (getline(&buffer, bufferSize, file) == -1) {
        // perror("Error getting new line. Exiting");
        // exit(1);
        return NULL;
    }

    char** tokens = (char**)malloc(sizeof(char*) * *tokenAmount);
    if (tokens == NULL) {
        perror("Error allocating tokens array");
        exit(1);
    }

    // Alternativelly, a null pointer may be specified, in which case the function
    //continues scanning where a previous successful call to the function ended.
    char* token;
    unsigned int i = 0;
    for (i = 0, token = strtok(buffer, separator); token != NULL && i < *tokenAmount; token = strtok(NULL, separator), i++) {
        tokens[i] = token;
    }

    // Remove trailing '\n'
    size_t lastStringLen = strlen(tokens[--i]);
    tokens[i][lastStringLen - 1] = '\0';
    return tokens;
}
