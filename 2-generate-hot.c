
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100


int generate_hv_from_samples(FILE *sPtr, FILE *hvPtr);


int main()
{
    FILE *hvPtr, *sPtr;

    // Generating hot vectors for training samples
    sPtr = fopen("trainingsamples.txt", "r");
    hvPtr = fopen("traininghv.txt", "w");
    generate_hv_from_samples(sPtr, hvPtr);
    fclose(sPtr);
    fclose(hvPtr);

    // Generating hot vectors for test samples
    sPtr = fopen("testingsamples.txt", "r");
    hvPtr = fopen("testinghv.txt", "w");
    generate_hv_from_samples(sPtr, hvPtr);
    fclose(sPtr);
    fclose(hvPtr);   

    return 0;
}


int generate_hv_from_samples(FILE *sPtr, FILE *hvPtr)
{
    char **dict, ch, word[MAX_WORD];
    int unique_words, *hotVector, word_i = 0;
    FILE *dPtr;

    dPtr = fopen("dictionary.txt", "r");

    if (hvPtr == NULL || dPtr == NULL || sPtr == NULL)
    {
        puts("!! Unable to access 'traininghv.txt' or 'dictionary.txt' or 'trainingsamples.txt'\n");
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
            fseek(hvPtr, -1, SEEK_CUR);
            fputc('\n', hvPtr);

            word[word_i] = '\0';
            word_i = 0;
        }

        // printf("%s\n", word);
    }
    while (ch != EOF);

    fclose(dPtr);
    return 0;
}