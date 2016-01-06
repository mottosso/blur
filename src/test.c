#define _XOPEN_SOURCE
#include <math.h>
#include <stdio.h>

int main(void)
{
    double sigma = 1;
    int W = 5;
    double kernel[W * W];
    double mean = W/2;
    double sum = 0.0; // For accumulating the kernel values

    for (int x = 0, i = 0; x < W; ++x, i++)
        for (int y = 0; y < W; ++y, i++) {
            kernel[i] = 
                exp(-0.5 * (pow((x-mean) / sigma, 2.0)
                          + pow((y-mean) / sigma, 2.0)))
                / (2 * M_PI * sigma * sigma);

            // Accumulate the kernel values
            sum += kernel[i];
        }

    // Normalize the kernel
    for (int x = 0, i = 0; x < W; ++x, i++) 
        for (int y = 0; y < W; ++y, i++)
            kernel[i] /= sum;

    for (int x = 0, i = 0; x < W; x++, i++)
    {
        for (int y = 0; y < W; y++, i++)
            printf("%f ", kernel[i]);

        printf("\n");
    }
}