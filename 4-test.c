
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int getD();


int main()
{
    FILE *testSamplesPtr, *modelPtr, *testTruesPtr;
    double *parameters;

    testSamplesPtr = fopen("testingsamples.txt", "r");
    modelPtr = fopen("model.txt", "r");
    testTruesPtr = fopen("testingtrues.txt", "r");

    if (testSamplesPtr == NULL || modelPtr == NULL || testTruesPtr == NULL) 
    {
        puts("!! Unable to open files");
        return -1;
    }



    return 0;
}


// Get number of unique words (Length of parameters)
int getD()
{
    FILE *dPtr = fopen("dictionary.txt", "r");
    int D;

    if (dPtr == NULL) {
        puts("!! Unable to read 'dictionary.txt'");
        return -1;
    }
    
    fscanf(dPtr, "%d", &D);
    fclose(dPtr);

    return D;
}


// Get number of test samples
int getN()
{
    FILE *tsPtr = fopen("testingsamples.txt", "r");
    int N = 0;
    char ch;

    if (tsPtr == NULL) {
        puts("!! Unable to read 'testingsamples.txt'");
        return -1;
    }
    
    do {
        ch = fgetc(tsPtr);
        if (ch == '\n') N++;
    }

    fclose(tsPtr);
    return N;
}