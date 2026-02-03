#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <measurements-path>\n", argv[0]);
        return 1;
    }

    char* filepath = argv[1];
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filepath);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    printf("File %s opened successfully\n", filepath);
    printf("File size: %ld bytes\n", size);

    fclose(file);
    return 0;
}