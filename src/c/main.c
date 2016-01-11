#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> // F_OK

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "blur.h"
#include "cli.h"
#include "helpers.h"


int main(int argc, char **argv)
{
    /* Command-line argument default values */
    char *filenameIn = NULL;
    char *filenameOut = NULL;
    double radius = 1;
    int x = 0,
        y = 0,
        size = 80,
        kernelSize = 5;

    if (!parseArgs(argc, argv, &filenameIn, &filenameOut,
                   &x, &y, &size, &kernelSize, &radius))
    {
        return 1;
    }

    if (size < kernelSize)
    {
        printf("Size too small.\n");
        return 1;
    }

    /* Load an image into memory, and set aside memory for result */
    int width, height, comp;
    uint8_t *pixelsIn = stbi_load(filenameIn, &width, &height, &comp, 0);
    uint8_t *pixelsOut = (uint8_t *) malloc(
        height * width * comp * sizeof(uint8_t *));

    if (pixelsIn == NULL)
    {
        printf("Could not load \"%s\".\n", filenameIn);
        free(pixelsOut);
        return 1;
    }

    if (pixelsOut == NULL)
    {
        printf("Could not allocate enough memory.\n");
        free(pixelsIn);
        return 1;
    }

    memcpy(pixelsOut, pixelsIn, height * width * comp * sizeof(uint8_t));

    /* Clamp x and y to available space */
    x = x > width ? width : x;
    y = y > height ? height : y;

    double *kernel = (double *) malloc(kernelSize * kernelSize * sizeof(double));
    double sum = computeKernel(kernel,
                  kernelSize,
                  radius  // sigma
    );

    normalise(kernel, sum, kernelSize, kernelSize);

    convolve(width,
             height,
             x,          // Define box
             y,          //
             x + size,   //
             y + size,   //
             comp,       // components
             pixelsIn,   // in
             pixelsOut,  // out
             kernel,     // kernel
             kernelSize  // kernelSize
    );

    if (stbi_write_png(filenameOut, width,
                       height, comp, pixelsOut, 0) == 0)
    {
        printf("Could not write \"%s\"\n", filenameOut);
    }
    else
    {
        printf("Wrote: %s (%ix%ix%i) " \
                         "(x=%i, y=%i, size=%i) " \
                         "to %s\n",
            filenameIn, height, width, x, y, size, comp,
            filenameOut);
    }

    free(pixelsIn);
    free(pixelsOut);
    free(filenameIn);
    free(filenameOut);

    return 0;
}
