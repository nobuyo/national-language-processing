#include <mecab.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define MAX_FILE_NUM 100

struct idfStruct
{
    char *word;
    int df;
    float idfWeight;
    struct idfStruct *nextAddr;
};

struct tfStruct
{
    char *word;
    int tf;
    float tfWeight;
    float tfidfWeight;
    struct tfStruct *nextAddr;
};

int main() {
    char *word;
    char string[256];
    char inputFileName[128];
    int i;
    int foundFlag;
    int fileNum;
    int listLength = 0;
    int tfVol = 0;
    float tfWeight = 0;
    int dfVol;
    float idfWeight;
    struct idfStruct *idfThis;
    struct idfStruct *idfNew;
    struct idfStruct *idfList = NULL;
    struct tfStruct *tfThis;
    struct tfStruct *tfNew;
    struct tfStruct *tfPre;
    struct tfStruct *tfTemp;
    struct tfStruct *tfList = NULL;

    FILE *textFile, *outputFile;

    for (fileNum = 1; fileNum <= MAX_FILE_NUM; fileNum++) {

        sprintf(inputFileName, "./tf_data/tf_%03d.txt", fileNum);
        if((textFile = fopen(inputFileName, "r"))== NULL) {
            printf("Can't Open File");
            exit;
        }

        while (fscanf(textFile, "%s\t%d\t%f\n", string, &tfVol, &tfWeight) != EOF)
        {
            word = (char *)malloc(sizeof(char) * (strlen(string) + 1));
            strncpy(word, string, strlen(string));
            word[strlen(string)+1] = '\0';

            foundFlag = 0;
            tfThis = tfList;
            // while(1)
            // {
            //     if(tfThis == NULL) {
            //         break;
            //     }
            //     else if(strcmp(tfThis->word, word) == 0) {
            //         foundFlag = 1;
            //         break;
            //     }
                
            //     if(tfThis->nextAddr == NULL) {
            //         break;
            //     }
            //     else {
            //         tfThis = tfThis->nextAddr;
            //     }
            // }

            // if(foundFlag == 1) {
            //     continue;
            // }
            // else {
                tfNew = (struct tfStruct *)malloc(sizeof(struct tfStruct));
                tfNew->word = (char *)malloc(sizeof(char) * strlen(word));
                tfNew->word = word;
                tfNew->tf = tfVol;
                tfNew->tfWeight = tfWeight;
                tfNew->nextAddr = NULL;

                if(tfList == NULL) {
                    tfList = tfNew;
                }
                else {
                    tfThis->nextAddr = tfNew;
                }
            // }
        }

        strcpy(inputFileName, "./idf.txt");
        if((textFile = fopen(inputFileName, "r"))== NULL) {
            printf("Can't Open File");
            exit;
        }

        while (fscanf(textFile, "%s\t%d\t%f\n", string, &dfVol, &idfWeight) != EOF)
        {
            word = (char *)malloc(sizeof(char) * (strlen(string) + 1));
            strncpy(word, string, strlen(string));
            word[strlen(string)+1] = '\0';

            // foundFlag = 0;
            // idfThis = idfList;
            // while(1)
            // {
            //     if(idfThis == NULL) {
            //         break;
            //     }
            //     else if(strcmp(idfThis->word, word) == 0) {
            //         foundFlag = 1;
            //         break;
            //     }
                
            //     if(idfThis->nextAddr == NULL) {
            //         break;
            //     }
            //     else {
            //         idfThis = idfThis->nextAddr;
            //     }
            // }

            // if(foundFlag == 1) {
            //     idfThis->count++;
            // }
            // else {
                idfNew = (struct idfStruct *)malloc(sizeof(struct idfStruct));
                idfNew->word = (char *)malloc(sizeof(char) * strlen(word));
                idfNew->word = word;
                idfNew->count = 1;
                idfNew->nextAddr = NULL;

                if(idfList == NULL) {
                    idfList = idfNew;
                }
                else {
                    idfThis->nextAddr = idfNew;
                }
            // }
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