#include <mecab.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define CHECK(eval) if (! eval) { \
        fprintf(stderr, "Exception:%s\n", mecab_strerror(mecab)); \
        mecab_destroy(mecab); \
        return -1; }

typedef struct
{
    char *word;
    int count;
    struct wordsStruct *nextAddr;
} wordsStruct;


int main(int argc, char **argv)    {
    char input[] = "すもももももももものうち";
    mecab_t *mecab;
    const mecab_node_t *node;
    const char *result;
    int i;
    size_t len = 3;
    const mecab_dictionary_info_t *d;
    char fileName[512] = "";
    FILE *file;
    int fileSize;
    char *string, *word;
    char ch;
    int foundFlag;
    wordsStruct *wordsList, *wordsThis, *wordsNew, *wordsPre, *wordsTemp;

    strcpy(fileName, "./data/001.txt");
    if((file = fopen(fileName, "r"))== NULL) {
        printf("Can't Open File",fileName);
        exit;
    }
    else {
        fseek( file, 0, SEEK_END );
        fileSize = ftell( file );
        printf( "%s FileSize:%d Byte\n", fileName, fileSize );
        fseek(file, 0, SEEK_SET); 
    }

    string = (char *)malloc(sizeof(char) * (fileSize+1));
    for(i=0; (ch = fgetc(file)) != -1;i++) {
        string[i] = ch;
    }
    string[i] = '\0';

    mecab = mecab_new(argc, argv);
    CHECK(mecab);

    node = mecab_sparse_tonode(mecab, string);
    CHECK(node);
    for (; node; node = node->next) {
        if (node->stat == MECAB_NOR_NODE || node->stat == MECAB_UNK_NODE) {
            fwrite(node->surface, sizeof(char), node->length, stdout);
            printf("\t%s\n", node->feature);

            word = (char *)malloc(sizeof(char) * (node->length +1));
            strncpy(word,node->surface, node->length);
            word[node->length] = '\0'; 

            printf("%s\n", word);

            foundFlag = 0;
            wordsThis = wordsList;
            while(1)
            {
                if(wordsThis == NULL) {
                    break;
                }
                else if(strcmp(wordsThis->word,word) == 0) {
                    foundFlag = 1;
                    free(word);
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
                wordsNew = (wordsStruct *)malloc(sizeof wordsStruct);
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

            int listLength = 0;
            if(wordsList->word != NULL) {
                wordsThis = wordsList;
                while(1) {
                    printf("%s:%d¥n",wordsThis->word,wordsThis->count);
                    listLength++;
                    if(wordsThis->nextAddr == NULL) {
                        break;
                    }
                    else {
                        wordsThis = wordsThis->nextAddr;
                    }
                }
            }
        }
    }

    
    mecab_destroy(mecab);

    return 0;
}