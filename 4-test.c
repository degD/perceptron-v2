
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int getD();
int getN();
double multiplication_of_wx(int *singleHotVector, double *parameters);


int main()
{
    int D, N, **hotVectors, *y_true, number_of_successful = 0;
    FILE *hvPtr, *modelPtr, *testTruesPtr;
    double *parameters, y_hat;
    char ch;

    D = getD();
    N = getN();
    // printf("%d, %d\n", D, N);

    hvPtr = fopen("testinghv.txt", "r");
    modelPtr = fopen("model.txt", "r");
    testTruesPtr = fopen("testingtrues.txt", "r");

    if (hvPtr == NULL || modelPtr == NULL || testTruesPtr == NULL) 
    {
        puts("!! Unable to open files");
        return -1;
    }

    // Read parameters
    parameters = calloc(D, sizeof(double));
    for (int i = 0; i < D; i++) {
        fscanf(modelPtr, "%lf", &parameters[i]);
        // printf("%3d: % lf\n", i, parameters[i]);
    }
    fclose(modelPtr);

    // Read hot vectors
    hotVectors = calloc(N, sizeof(int *));
    for (int i = 0; i < N; i++) 
    {
        hotVectors[i] = calloc(D, sizeof(int));
        for (int j = 0; j < D; j++) fscanf(hvPtr, "%d", &hotVectors[i][j]);
    }
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < D; j++) printf("%d ", hotVectors[i][j]);
    //     puts("");
    // }
    fclose(hvPtr);

    // Read true "y" values
    y_true = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) 
    {
        ch = fgetc(testTruesPtr);
        fgetc(testTruesPtr);
        
        if (ch == '+') y_true[i] = +1;
        else y_true[i] = -1;
    }
    fclose(testTruesPtr);

    // Test the model
    for (int i = 0; i < N; i++) 
    {
        y_hat = tanh(multiplication_of_wx(hotVectors[i], parameters));
        printf("% lf, %d\n", y_hat, y_true[i]);
        y_hat = y_hat > 0 ? 1 : -1;
        if (y_hat == y_true[i]) number_of_successful++;
    }
    printf("Number of successful predictions: %d/%d\n", number_of_successful, N);
    printf("Prediction accuracy: %.2lf%%\n", 100 * ((double)number_of_successful / N));

    for (int i = 0; i < N; i++) free(hotVectors[i]);
    free(hotVectors);
    free(y_true);

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


// Get number of test samples (hot vectors)
int getN()
{
    FILE *hvPtr = fopen("testinghv.txt", "r");
    int N = 0;
    char ch;

    if (hvPtr == NULL) {
        puts("!! Unable to read 'testinghv.txt'");
        return -1;
    }
    
    do {
        ch = fgetc(hvPtr);
        if (ch == '\n') N++;
    }
    while(ch != EOF);

    fclose(hvPtr);
    return N;
}


double multiplication_of_wx(int *singleHotVector, double *parameters)
{
    int D = getD();
    double m = 0;
    for (int i = 0; i < D; i++) m += singleHotVector[i] * parameters[i];
    return m;
}