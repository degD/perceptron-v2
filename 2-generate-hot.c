
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100

int main()
{
    char **dict, ch, word[MAX_WORD];
    int unique_words, *hotVector, word_i = 0;
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
        fscanf(dPtr, "%s", dict[i]);
        // printf("%3d: %s\n", i, dict[i]);
    }

    // Generate and save hot vectors
    hotVector = calloc(unique_words, sizeof(int));
    do {
        ch = fgetc(sPtr);

        if (ch == ' ' || ch == '\n' || ch == EOF)
        {
            word[word_i] = '\0';
            word_i = 0;

            for (int i = 0; i < unique_words; i++)
            {
                if (strcmp(word, dict[i]) == 0)
                {
                    hotVector[i] = 1;
                    break;
                }
            }
        }
        else word[word_i++] = ch;

        if (ch == '\n' || ch == EOF)
        {
            for (int i = 0; i < unique_words; i++) {
                fprintf(hvPtr, "%d ", hotVector[i]);
                hotVector[i] = 0;
            }
            fputc('\n', hvPtr);

            word[word_i] = '\0';
            word_i = 0;
        }

        // printf("%s\n", word);
    }
    while (ch != EOF);

    
    fclose(dPtr);
    fclose(hvPtr);
    return 0;
}