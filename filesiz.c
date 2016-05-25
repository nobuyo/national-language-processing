#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char fileName[512] = "";
    FILE *file;
    int fileSize;

    strcpy(fileName, "./data/001.txt");
    if((file = fopen(fileName, "r"))== NULL) {
        printf("Can't Open File",fileName);
        exit;
    }
    else {
        fseek( file, 0, SEEK_END );
        fileSize = ftell( file );
        printf( "%s FileSize:%d Byte\n", fileName, fileSize );
    }
}