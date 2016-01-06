#define _XOPEN_SOURCE

#include <stdint.h>
#include <stdbool.h>

#include "blur.h"

int extract(uint8_t *in,
            uint8_t *out,
            int x,
            int y,
            int sourceWidth,
            int sourceHeight,
            int targetWidth,
            int targetHeight,
            int components)
{
  if (y + targetHeight > sourceHeight || x + targetWidth > sourceWidth)
  {
    return 1;
  }

  for (int col = 0, i = 0; col < targetHeight; col++)
  {
    for (int row = 0; row < targetWidth; row++)
    {
      /* Crop input

        __x__________
      y|__|___       |
       |  |_|_|      |
       |  |_|_|      |
       |             |
       |_____________|

      */

      for (int c = 0; c < components; c++)
      {
        /* Per component (e.g. RGB)
           _____
          |     |_
          |     | |_
          |_____| | |
            |_____| |
              |_____|

        */
        size_t index = (size_t) 0
          + (y + col) * sourceWidth * components  // Offset rows
          + (x + row) * components                // Offset columns
          + c;                                    // Offset components
        out[i] = in[index];
        i++;
      }
    }
  }

  return 0;
}

int integrate(uint8_t *in,
              uint8_t *out,
              int x,
              int y,
              int sourceWidth,
              int sourceHeight,
              int targetWidth,
              int targetHeight,
              int components)
{
  if (y + sourceHeight > targetHeight || x + sourceWidth > targetWidth)
  {
    return 1;
  }

  for (int col = 0, i = 0; col < sourceHeight; col++)
  {
    for (int row = 0; row < sourceWidth; row++)
    {
      /* Find destination of trimmed chunk

        __x__________
      y|__|___       |
       |  |_|_|      |
       |  |_|_|      |
       |             |
       |_____________|

      */

      for (int c = 0; c < components; c++)
      {
        /* Per component (e.g. RGB)
           _____
          |     |_
          |     | |_
          |_____| | |
            |_____| |
              |_____|

        */
        size_t index = (size_t) 0
          + (y + col) * targetWidth * components  // Offset rows
          + (x + row) * components                // Offset columns
          + c;                                    // Offset components
        out[index] = in[i];
        i++;
      }
    }
  }

  return 0;
}

bool convolve(int width, int height, int components,
              uint8_t *in, uint8_t *out, int *kernel, int kernelSize)
{

  /* Only deal with kernels of odd numbered dimensions */
  if (kernelSize % 2 != 1)
  {
    return false;
  }

  /* Keep track of current incoming and outgoing pixels */
  uint8_t *inPixel = in;
  uint8_t *outPixel = out;

  int kernelSum = 0;
  for (int i = 0; i < kernelSize * kernelSize; i++)
      kernelSum += kernel[i];

  for (int h = 0; h < height; h++)
  {
    for (int w = 0; w < width; w++ )
    {
      /* Account for border
       * 
       * Each pixel in the border consist of a duplicate pixel from its
       * nearest neighbour. The larger the kernel, the more duplication occurs.
       */
      int margin = (kernelSize - 1) / 2;
      int xoffset = w < margin ? margin : w > (width - margin - 1) ? -margin : 0;
      int yoffset = h < margin ? margin : h > (height - margin - 1) ? -margin : 0;

      /* Mix each component separately */
      for (int component = 0; component < components; component++)
      {
        int sum = 0;

        for (int row = 0; row < kernelSize; row++)
        {
          for (int col = 0; col < kernelSize; col++)
          {
            /* Calculate kernel counterpart

            The counterpart, referred to as "sample", originates at
            the current pixel component and spans out in every direction
            until a matrix of equivalent dimension is found. This matrix
            is then multiplied with the kernel to produce the resuting pixel.

            The origin between the kernel and sample position differ

            kernel          sample
             ___________     ___________
            |k  |   |   |   |   |   |   |
            |___|___|___|   |___|___|___|
            |   |   |   |   |   |s  |   |
            |___|___|___|   |___|___|___|
            |   |   |   |   |   |   |   |
            |___|___|___|   |___|___|___|

            */

            int *kernelPixel = kernel
              + (row) * kernelSize        // Offset to previous/next row(s)
              + (col);                    // Offset to previous/next column(s)

            uint8_t *samplePixel = inPixel                      // Base pixel
              + (row - margin + yoffset) * width * components   // Offset rows
              + (col - margin + xoffset) * components;          // Offset columns

            sum += *kernelPixel * samplePixel[component];
          }
        }
        outPixel[component] = sum / kernelSum;
      }

    inPixel += components;
    outPixel += components;

    }
  }

  return true;
}

double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}
