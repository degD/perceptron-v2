
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100

int main()
{
    char **dict, ch, word[MAX_WORD];
    int unique_words, *hotVector;
    FILE *dPtr, *hvPtr, *sPtr;

    dPtr = fopen("dictionary.txt", "r");
    sPtr = fopen("samples.txt", "r");
    hvPtr = fopen("hotVectors.txt", "w");

    if (hvPtr == NULL || dPtr == NULL || sPtr == NULL)
    {
        puts("!! Unable to access 'hotVectors.txt' or 'dictionary.txt' or 'samples.txt'\n");
        return -1;
    }

    // Initialize and read dictionary
    fscanf(dPtr, "%d", &unique_words);
    dict = calloc(unique_words, sizeof(char *));

    for (int i = 0; i < unique_words; i++)
    {
        dict[i] = calloc(MAX_WORD, sizeof(char));
        fgets(dict[i], MAX_WORD, dPtr);
    }

    // Generate and save hot vectors
    hotVector = calloc(unique_words, sizeof(int));
    do {
        fscanf(sPtr, "%s", word);
        printf("%s\n", word);
    }
    while (!feof(sPtr));

    
    fclose(dPtr);
    fclose(hvPtr);
    return 0;
}