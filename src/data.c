#include "../lib/data.h"

long double** loadData(FILE* file, const char* separator) {
    char buffer[5000];
    size_t bufferSize = 0;

    size_t lineSize = getLineSize(file, separator, buffer, &bufferSize);

    size_t linesCount = countLines(file);
    char** dataLine[linesCount];

    printf("lineSize %lu linesCount %lu\n", lineSize, linesCount);

    for (int i = 0; i < linesCount; i++) {
        char** line = readLine(file, separator, buffer, &bufferSize, &lineSize);
        for (int j = 0; j < lineSize; j++) {
            printf("Copiando %s\n", line[j]);
            dataLine[i][j] = (char*) malloc(sizeof(char) * (strlen(line[j])) + 1);
            strcpy(dataLine[i][j], line[j]);
        }
    }

    for (int i = 0; i < linesCount; i++) {
        for (int j = 0; j < lineSize; j++) {
            printf("%s ", dataLine[i][j]);
        }
    }
    return NULL;
}
