

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 5
#define D 6
#define EPS 0.001

double multiplication_of_wx(int *singleHotVector, double *parameters);
double *partial_derivative_of_mean_square_error(int *singleHotVector, double *parameters, int y_true);
void gradient_descent(int **hotVectors, double *parameters, double *y_true);


int main()
{
    FILE *hotVectorsPtr, *truePtr;
    int **hotVectors, *y_true, i, j;
    double *parameters;
    char ch;

    // Open hot vectors file
    hotVectorsPtr = fopen("hotVectors.txt", "r");
    if (hotVectorsPtr == NULL) 
    {
        puts("!! Unable to open file 'hotVectors.txt'");
        return -1;
    }   

    // Open true values file
    truePtr = fopen("trues.txt", "r");
    if (hotVectorsPtr == NULL) 
    {
        puts("!! Unable to open file 'trues.txt'");
        return -1;
    }    

    // Read hot vectors (NxD)
    hotVectors = calloc(N, sizeof(double *));
    for (int i = 0; i < D; i++) hotVectors[i] = calloc(D, sizeof(double));
    i = j = 0;
    while ((ch = getc(hotVectorsPtr)) != EOF) 
    {
        if (ch == '\n') i++;
        if (ch == '1' || ch == '0') 
        {
            hotVectors[i][j] = ch - '0';
            j++;
        }
    }
    fclose(hotVectorsPtr);

    // Read true y values
    y_true = calloc(N, sizeof(int));
    i = 0;
    while ((ch = getc(truePtr)) != EOF) 
    {
        if (ch == '+') 
        {
            y_true[i] = +1;
            i++;
        }
        if (ch == '-')
        {
            y_true[i] = -1;
            i++;            
        }
    }
    fclose(truePtr);

    // Initialize parameters
    parameters = calloc(D, sizeof(double));

    // 5 iteration of GD
    for (int i = 0; i < 5; i++) 
    {
        gradient_descent(hotVectors, parameters, y_true);
        for (int j = 0; j < D; j++) printf("%lf ", parameters[j]);
        puts("");
    }

    return 0;
}


void gradient_descent(int **hotVectors, double *parameters, double *y_true) 
{
    double *pdmse_sum = calloc(D, sizeof(double));
    sum_of_partial_derivative_mse(hotVectors, parameters, pdmse_sum, y_true);
    for (int i = 0; i < D; i++) 
        parameters[i] -= EPS * pdmse_sum[i];
    free(pdmse_sum);
}


double *partial_derivative_of_mean_square_error(int *singleHotVector, double *parameters, int single_y_true)
{
    double wx, *pdmse = calloc(D, sizeof(double));
    wx = multiplication_of_wx(singleHotVector, parameters);
    for (int i = 0; i < D; i++) 
        pdmse[i] = (single_y_true - tanh(wx)) * parameters[i] * (-1 / (cosh(wx) * cosh(wx)));
    return pdmse;
}


void sum_of_partial_derivative_mse(int **hotVectors, double *parameters, double *pdmse_sum, int *y_true)
{
    double *pdmse;
    for (int i = 0; i < N; i++)
    {
        pdmse = partial_derivative_of_mean_square_error(hotVectors[i], parameters, y_true[i]);
        for (int j = 0; j < D; j++) pdmse_sum[j] += pdmse[j];
        free(pdmse);
    }
}


double multiplication_of_wx(int *singleHotVector, double *parameters)
{
    double m = 0;
    for (int i = 0; i < D; i++) m += singleHotVector[i] * parameters[i];
    return m;
}
