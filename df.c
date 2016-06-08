#include <mecab.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define MAX_FILE_NUM 100

struct wordsStruct
{
    char *word;
    int count;
    struct wordsStruct *nextAddr;
};

int main() {
    char *word;
    char string[256];
    char inputFileName[128];
    int i;
    int foundFlag;
    int fileNum;
    int listLength = 0;
    float tfVol = 0;
    float tfWeight = 0;
    struct wordsStruct *wordsThis;
    struct wordsStruct *wordsNew;
    struct wordsStruct *wordsPre;
    struct wordsStruct *wordsTemp;
    struct wordsStruct *wordsList = NULL;

    FILE *textFile, *outputFile;


    for (fileNum = 1; fileNum <= MAX_FILE_NUM; fileNum++) {

        sprintf(inputFileName, "./tf_data/tf_%03d.txt", fileNum);
        if((textFile = fopen(inputFileName, "r"))== NULL) {
            printf("Can't Open File");
            exit;
        }

        while (fscanf(textFile, "%s\t%f\t%f\n", string, &tfVol, &tfWeight) != EOF)
        {
            word = (char *)malloc(sizeof(char) * (strlen(string) + 1));
            strncpy(word, string, strlen(string));
            word[strlen(string)+1] = '\0';

            foundFlag = 0;
            wordsThis = wordsList;
            while(1)
            {
                if(wordsThis == NULL) {
                    break;
                }
                else if(strcmp(wordsThis->word, word) == 0) {
                    foundFlag = 1;
                    break;
                }
                
                if(wordsThis->nextAddr == NULL) {
                    break;
                }
                else {
                    wordsThis = wordsThis->nextAddr;
                }
            }

            if(foundFlag == 1) {
                wordsThis->count++;
            }
            else {
                wordsNew = (struct wordsStruct *)malloc(sizeof(struct wordsStruct));
                wordsNew->word = (char *)malloc(sizeof(char) * strlen(word));
                wordsNew->word = word;
                wordsNew->count = 1;
                wordsNew->nextAddr = NULL;

                if(wordsList == NULL) {
                    wordsList = wordsNew;
                }
                else {
                    wordsThis->nextAddr = wordsNew;
                }
            }
        }
    }

    if(wordsList->word != NULL) {
        wordsThis = wordsList;
        while(1) {
            // printf("%s:%d\n", wordsThis->word,wordsThis->count);
            listLength++;
            if(wordsThis->nextAddr == NULL) {
                break;
            }
            else {
                wordsThis = wordsThis->nextAddr;
            }
        }
    }

    for(i=0; i < listLength; i++) {
        if(wordsList != NULL) {

            wordsThis = wordsList;
            wordsPre = NULL;

            while(1) {
                if(wordsThis->nextAddr == NULL) break;
                if(wordsThis->count < wordsThis->nextAddr->count) {
                    wordsTemp = wordsThis;
                    wordsThis = wordsTemp->nextAddr;
                    wordsTemp->nextAddr = wordsThis->nextAddr;
                    wordsThis->nextAddr = wordsTemp;

                    if(wordsPre == NULL) {
                        wordsList = wordsThis;
                    }
                    else {
                        wordsPre->nextAddr = wordsThis;
                    }
                }
                wordsPre = wordsThis;
                wordsThis = wordsThis->nextAddr;
            }
        }
    }

    if((outputFile = fopen("./idf.txt", "w"))== NULL) {
        printf("Can't Open File");
        exit;
    }
    
    if(wordsList->word != NULL) {
        wordsThis = wordsList;
        while(1) {
            fprintf(outputFile, "%s\t%d\t%f\n", wordsThis->word,wordsThis->count,log10((double)MAX_FILE_NUM/(double)wordsThis->count)+1);
            // printf("%s:%d\n", wordsThis->word,wordsThis->count);
            if(wordsThis->nextAddr == NULL) {
                break;
            }
            else {
                wordsThis = wordsThis->nextAddr;
            }
        }
    }

    return 0;

}