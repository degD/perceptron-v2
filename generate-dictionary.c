
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 100

int main()
{
    int word_in_dict, unique_words = 0, sample_word_i = 0, sample_word_count = 1;
    FILE *sPtr, *dPtr;
    char **dict, ch, sample_word[MAX_WORD];

    sPtr = fopen("samples.txt", "r");
    dPtr = fopen("dictionary.txt", "w");

    if (sPtr == NULL || dPtr == NULL)
    {
        puts("!! Unable to read 'samples.txt' or 'dictionary.txt'\n");
        return -1;
    }

    // Count number of words in samples
    while ((ch = fgetc(sPtr)) != EOF)
    {
        if (ch == ' ' || ch == '\n') sample_word_count++;
    }
    printf("%d\n", sample_word_count);

    // Allocate memory for dictionary
    dict = calloc(sample_word_count, sizeof(char *));
    for (int i = 0; i < sample_word_count; i++) 
    {
        dict[i] = calloc(MAX_WORD, sizeof(char));
        dict[i][0] = '\0';
    }

    // Find unique words
    rewind(sPtr);
    do {
        ch = fgetc(sPtr);

        if (ch == ' ' || ch == '\n' || ch == EOF)
        {
            sample_word[sample_word_i] = '\0';
            sample_word_i = 0;
            word_in_dict = 0;

            for (int i = 0; i < sample_word_count; i++) 
            {
                if (strcmp(sample_word, dict[i]) == 0) 
                {
                    word_in_dict = 1;
                    break;
                }
            }

            if (word_in_dict == 0)
            {
                for (int i = 0; i < sample_word_count; i++) 
                {
                    if (dict[i][0] == '\0') 
                    {
                        strcpy(dict[i], sample_word);
                        break;
                    }
                }
                unique_words++;
            }

        }
        else sample_word[sample_word_i++] = ch;

        // printf("%s\n", sample_word);
    } 
    while (ch != EOF);

    // printf("%d unique\n", unique_words);
    // for (int i = 0; i < unique_words; i++) printf("%s\n", dict[i]);

    

    fclose(sPtr);
    fclose(dPtr);
    return 0;
}
        