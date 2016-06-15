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
    char outputFileName[128];
    int i,j;
    int fileNum;
    int tfListLength = 0;
    int idfListLength = 0;
    int tfVol = 0;
    float tfWeight = 0;
    int dfVol = 0;
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

    strcpy(inputFileName, "./idf.txt");
    if((textFile = fopen(inputFileName, "r"))== NULL) {
        printf("Can't Open File");
        exit;
    }

    idfThis = idfList;

    while (fscanf(textFile, "%s\t%d\t%f\n", string, &dfVol, &idfWeight) != EOF)
    {
        word = (char *)malloc(sizeof(char) * (strlen(string) + 1));
        strncpy(word, string, strlen(string));
        word[strlen(string)+1] = '\0';


        idfNew = (struct idfStruct *)malloc(sizeof(struct idfStruct));
        idfNew->word = (char *)malloc(sizeof(char) * strlen(word));
        idfNew->word = word;
        idfNew->df = dfVol;
        idfNew->idfWeight = idfWeight;
        idfNew->nextAddr = NULL;

        idfListLength++;

        if(idfList == NULL) {
            idfList = idfNew;
            idfThis = idfList;
        }
        else {
            idfThis->nextAddr = idfNew;
            idfThis = idfThis->nextAddr;
        }
        // free(word);
    }

    fclose(textFile);

    for (fileNum = 1; fileNum <= MAX_FILE_NUM; fileNum++) {

        tfList = NULL;

        sprintf(inputFileName, "./tf_data/tf_%03d.txt", fileNum);
        if((textFile = fopen(inputFileName, "r"))== NULL) {
            printf("Can't Open File");
            exit;
        }

        tfThis = tfList;

        while (fscanf(textFile, "%s\t%d\t%f\n", string, &tfVol, &tfWeight) != EOF)
        {
            word = (char *)malloc(sizeof(char) * (strlen(string) + 1));
            strncpy(word, string, strlen(string));
            word[strlen(string)+1] = '\0';

            tfNew = (struct tfStruct *)malloc(sizeof(struct tfStruct));
            tfNew->word = (char *)malloc(sizeof(char) * strlen(word));
            tfNew->word = word;
            tfNew->tf = tfVol;
            tfNew->tfWeight = tfWeight;
            tfNew->nextAddr = NULL;

            tfListLength++;

            if(tfList == NULL) {
                tfList = tfNew;
                tfThis = tfList;
            }
            else {
                tfThis->nextAddr = tfNew;
                tfThis = tfThis->nextAddr;
            }

            // free(word);
        }


        // if(tfList->word != NULL) {
        //     tfThis = tfList;
        //     while(1) {
        //         listLength++;
        //         if(tfThis->nextAddr == NULL) {
        //             break;
        //         }
        //         else {
        //             tfThis = tfThis->nextAddr;
        //         }
        //     }
        // }

        // if (tfList->word != NULL)  tfThis = tfList;
        // if (idfList->word != NULL) idfThis = idfList;
        // while (tfThis != NULL) {
        //     if (tfThis == NULL)
        //         break;
        //     while (idfThis != NULL) {
        //         if (strcmp(tfThis->word,idfThis->word) == 0) {
        //             tfThis->tfidfWeight = tfThis->tfWeight * idfThis->idfWeight;
        //             break;
        //         }
        //         else {
        //             idfThis = idfThis->nextAddr;
        //         }
        //     }
        //     tfThis = tfThis->nextAddr;
        // }

        tfThis = tfList;
        for (i=0; i<tfListLength; i++) {
            idfThis = idfList;
            if (tfThis == NULL) break;
            for (j=0; j<idfListLength; j++) {
                if (idfThis == NULL) break;
                if (strcmp(tfThis->word, idfThis->word) == 0) {
                    tfThis->tfidfWeight = tfThis->tfWeight * idfThis->idfWeight;
                    break;
                }
                else {
                    idfThis = idfThis->nextAddr;
                }
            }
            tfThis = tfThis->nextAddr;
        }

        for(i=0; i < tfListLength; i++) {
            if(tfList != NULL) {

                tfThis = tfList;
                tfPre = NULL;

                while(1) {
                    if(tfThis->nextAddr == NULL) break;
                    if(tfThis->tfidfWeight < tfThis->nextAddr->tfidfWeight) {
                        tfTemp = tfThis;
                        tfThis = tfTemp->nextAddr;
                        tfTemp->nextAddr = tfThis->nextAddr;
                        tfThis->nextAddr = tfTemp;

                        if(tfPre == NULL) {
                            tfList = tfThis;
                        }
                        else {
                            tfPre->nextAddr = tfThis;
                        }
                    }
                    tfPre = tfThis;
                    tfThis = tfThis->nextAddr;
                }
            }
        }

        sprintf(outputFileName, "./tfidf_data/tfidf_%03d.txt", fileNum);
        if((outputFile = fopen(outputFileName, "w")) == NULL) {
            printf("Can't Open File");
            exit;
        }
        
        if(tfList->word != NULL) {
            tfThis = tfList;
            while(1) {
                fprintf(outputFile, "%s\t%d\t%f\t%f\n", tfThis->word,tfThis->tf,tfThis->tfWeight,tfThis->tfidfWeight);
                // printf("%s\n", idfThis->word);
                if(tfThis->nextAddr == NULL) {
                    break;
                }
                else {
                    tfThis = tfThis->nextAddr;
                }
            }
        }

        fclose(outputFile);
    }

    return 0;

}