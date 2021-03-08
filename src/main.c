#include "../lib/fileReader.h"

int main(int argc, char** argv) {
    FILE* file = openFile(argv[1]);
    size_t bufferSize = 2000;
    char buffer[2000];
    size_t tokenAmount = getLineSize(file, ",", buffer, &bufferSize);
    char** tokens = readLine(file, ",", buffer, &bufferSize, &tokenAmount);

    for (int i = 0; i < tokenAmount; i++) {
        printf("%s ", tokens[i]);
    }
    puts("");

    closeFile(file);
    free(tokens);
}
