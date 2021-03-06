#include <mecab.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define CHECK(eval) if (! eval) { \
        fprintf(stderr, "Exception:%s\n", mecab_strerror(mecab)); \
        mecab_destroy(mecab); \
        return -1; }

#define MAX_FILE_NUM 100

struct wordsStruct
{
    char *word;
    int count;
    struct wordsStruct *nextAddr;
};


int main(int argc, char **argv)    {
    mecab_t *mecab;
    const mecab_node_t *node;
    const char *result;
    int i;
    size_t len = 3;
    const mecab_dictionary_info_t *d;
    FILE *file, *input, *output;
    int fileSize;
    char *string, *word;
    char ch;
    int foundFlag = 0;
    int listLength = 0;
    int totalCount = 0;
    int fileNum;
    struct wordsStruct *wordsThis, *wordsNew, *wordsPre, *wordsTemp;
    char inputFileName[128], outputFileName[128];

    for (fileNum = 1; fileNum <= MAX_FILE_NUM; fileNum++) {

        struct wordsStruct *wordsList = { NULL };

        listLength = 0;
        totalCount = 0;
        sprintf(inputFileName, "./data/%03d.txt", fileNum);
        sprintf(outputFileName, "./tfmod_data/tfmod_%03d.txt", fileNum);

        if((input = fopen(inputFileName, "r"))== NULL) {
            printf("Can't Open File", inputFileName);
            exit;
        }
        else {
            fseek( input, 0, SEEK_END );
            fileSize = ftell( input );
            // printf( "%s FileSize:%d Byte\n", inputFileName, fileSize );
            fseek(input, 0, SEEK_SET); 
        }

        string = (char *)malloc(sizeof(char) * (fileSize+1));
        for(i=0; (ch = fgetc(input)) != -1;i++) {
            string[i] = ch;
        }
        string[i] = '\0';

        fclose(input);

        mecab = mecab_new(argc, argv);
        CHECK(mecab);

        node = mecab_sparse_tonode(mecab, string);
        CHECK(node);
        for (; node; node = node->next) {
            if (node->stat == MECAB_NOR_NODE || node->stat == MECAB_UNK_NODE) {
                fwrite(node->surface, sizeof(char), node->length, stdout);
                printf("\t%s\n", node->feature);

                word = (char *)malloc(sizeof(char) * (node->length + 1));
                strncpy(word, node->surface, node->length);
                word[node->length] = '\0'; 

                foundFlag = 0;
                wordsThis = wordsList;
                if (node->posid >= 36 && node->posid <= 67 && node->length > 1) {
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
                        free(word);
                    }
                    else {
                        wordsNew = (struct wordsStruct *)malloc(sizeof(struct wordsStruct));
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
        }

        if(wordsList->word != NULL) {
            wordsThis = wordsList;
            while(1) {
                // printf("%s:%d\n", wordsThis->word,wordsThis->count);
                listLength++;
                totalCount+=wordsThis->count;
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

        if((output = fopen(outputFileName, "w"))== NULL) {
            printf("Can't Open File", outputFileName);
            exit;
        }

        if(wordsList->word != NULL) {
            wordsThis = wordsList;
            while(1) {
//                printf(         "%s\t%d\t%f\n", wordsThis->word, wordsThis->count ,(float)wordsThis->count/(float)totalCount);
                fprintf(output, "%s\t%d\t%f\n", wordsThis->word, wordsThis->count ,(float)wordsThis->count/(float)totalCount);
                if(wordsThis->nextAddr != NULL) {
                    wordsThis = wordsThis->nextAddr;
                }
                else {
                    break;
                }
            }
        }

        free(wordsList);
        fclose(output);
        mecab_destroy(mecab);
    }

    return 0;
}