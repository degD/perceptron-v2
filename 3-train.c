
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define N 6
#define D 22
#define EPS 0.1
#define CONVERGENCE_LIMIT 0.00001
#define STEP_LIMIT 1000

double multiplication_of_wx(int *singleHotVector, double *parameters);
double *partial_derivative_of_mean_square_error(int *singleHotVector, double *parameters, int y_true);
void sum_of_partial_derivative_mse(int **hotVectors, double *parameters, double *pdmse_sum, int *y_true);
void gradient_descent(int **hotVectors, double *parameters, int *y_true);
double total_mean_square_error(int **hotVectors, double *parameters, int *y_true);
void stochastic_gradient_descent(int **hotVectors, double *parameters, int *y_true);
double *stochastic_partial_derivative_mse(int **hotVectors, double *parameters, int *y_true);
int adaptive_movement_estimation_adam(int **hotVectors, double *parameters, int *y_true);


int main()
{
    FILE *hotVectorsPtr, *truePtr, *modelPtr;
    int **hotVectors, *y_true, i, j;
    double *parameters;
    double total_mse = 1, total_mse_old = 0;
    char ch;
    int mode, step = 0;

    printf("Which algorithm? (GD: 0, SGD: 1, ADAM: 2): ");
    scanf("%d", &mode);
    if (mode < 0 || mode > 2) {
        puts("!! Undefined algorithm");
        return -1;
    }

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

    // Open model file
    modelPtr = fopen("model.txt", "w");
    if (modelPtr == NULL) 
    {
        puts("!! Unable to open file 'model.txt'");
        return -1;
    }   

    // Read hot vectors (NxD)
    hotVectors = calloc(N, sizeof(int *));
    for (int i = 0; i < N; i++) hotVectors[i] = calloc(D, sizeof(int));
    i = j = 0;
    while ((ch = getc(hotVectorsPtr)) != EOF) 
    {
        // printf("%d %d %d\n", i, j, ch);
        if (ch == '\n') 
        {
            i++;
            j = 0;
        }
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
    for (int i = 0; i < D; i++) parameters[i] = 0.1;

    // Gradient Descent
    if (mode == 0)
    {
        // Repeat until converges or step number exceeds the limit
        while (fabs(total_mse - total_mse_old) > CONVERGENCE_LIMIT && step < STEP_LIMIT)
        {
            gradient_descent(hotVectors, parameters, y_true);
            total_mse_old = total_mse;
            total_mse = total_mean_square_error(hotVectors, parameters, y_true);

            // for (int j = 0; j < D; j++) printf("% lf ", parameters[j]);
            // printf("-> %lf\n", fabs(total_mse - total_mse_old));
        }
    }

    // Stochastic Gradient Descent
    if (mode == 1)
    {
        // Repeat until converges or step number exceeds the limit
        while (fabs(total_mse - total_mse_old) > CONVERGENCE_LIMIT && step < STEP_LIMIT)
        {
            stochastic_gradient_descent(hotVectors, parameters, y_true);
            total_mse_old = total_mse;
            total_mse = total_mean_square_error(hotVectors, parameters, y_true);

            // for (int j = 0; j < D; j++) printf("% lf ", parameters[j]);
            // printf("-> %lf\n", fabs(total_mse - total_mse_old));
        }
    }

    // ADAM
    if (mode == 2)
        adaptive_movement_estimation_adam(hotVectors, parameters, y_true);


    // Save trained parameters and free memory
    puts("Complete, saving...");
    for (int i = 0; i < D; i++) fprintf(modelPtr, "%lf\n", parameters[i]);
    free(parameters);
    
    for (int i = 0; i < N; i++) free(hotVectors[i]);
    free(hotVectors);
    free(y_true);

    fclose(modelPtr);
    return 0;
}


void gradient_descent(int **hotVectors, double *parameters, int *y_true) 
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
        pdmse[i] = (single_y_true - tanh(wx)) * singleHotVector[i] * (-1 / (cosh(wx) * cosh(wx)));
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


double total_mean_square_error(int **hotVectors, double *parameters, int *y_true)
{
    double r, mse_sum = 0;
    for (int i = 0; i < N; i++) 
    {
        r = (y_true[i] - tanh(multiplication_of_wx(hotVectors[i], parameters)));
        mse_sum += r * r;
    }
    return (1.0 / N) * mse_sum;
}


void stochastic_gradient_descent(int **hotVectors, double *parameters, int *y_true) 
{
    double *approx_pdmse;
    approx_pdmse = stochastic_partial_derivative_mse(hotVectors, parameters, y_true);

    for (int i = 0; i < D; i++) 
        parameters[i] -= EPS * approx_pdmse[i];
    free(approx_pdmse);
}


double *stochastic_partial_derivative_mse(int **hotVectors, double *parameters, int *y_true)
{
    int random_i = rand() % N;
    return partial_derivative_of_mean_square_error(hotVectors[random_i], parameters, y_true[random_i]);
}


int adaptive_movement_estimation_adam(int **hotVectors, double *parameters, int *y_true)
{
    double total_mse = 1, total_mse_old = 0;
    double *m, *v, *approx_gradient, *m_hat, *v_hat;
    double alpha = 0.001, beta1 = 0.9, beta2 = 0.999, e = 0.000001;
    int t = 0, step_count = 0;

    m = calloc(D, sizeof(double));
    v = calloc(D, sizeof(double));
    m_hat = calloc(D, sizeof(double));
    v_hat = calloc(D, sizeof(double));

    while (fabs(total_mse - total_mse_old) > CONVERGENCE_LIMIT && t < STEP_LIMIT)
    {
        t += 1;
        approx_gradient = stochastic_partial_derivative_mse(hotVectors, parameters, y_true);

        for (int i = 0; i < D; i++)
        {
            m[i] = (beta1 * m[i]) + ((1 - beta1) * approx_gradient[i]);
            v[i] = (beta2 * v[i]) + ((1 - beta2) * approx_gradient[i] * approx_gradient[i]);
            m_hat[i] = m[i] / (1 - pow(beta1, t));
            v_hat[i] = v[i] / (1 - pow(beta2, t));
            parameters[i] -= alpha * m_hat[i] / (sqrt(v_hat[i]) + e);
        }

        total_mse_old = total_mse;
        total_mse = total_mean_square_error(hotVectors, parameters, y_true);

        // for (int j = 0; j < D; j++) printf("% lf ", parameters[j]);
        // printf("%4d -> %lf\n", step_count, total_mse);        

        step_count++;
    }

    return step_count;
}
