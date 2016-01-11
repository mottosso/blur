#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "blur.h"

bool convolve(const int width,  
              const int height,
              const int minX,
              const int minY,
              const int maxX,
              const int maxY,
              const int components,
              const uint8_t *in,
              uint8_t *out,
              const double *kernel,
              const int kernelSize)
{

  /* Only deal with kernels of odd numbered dimensions */
  if (kernelSize % 2 != 1)
  {
    return false;
  }

  /* Keep track of current incoming and outgoing pixels */
  const uint8_t *inPixel = in;
  uint8_t *outPixel = out;

  int areaSize = maxX - minX;
  int areaCenter = areaSize / 2;

  double *kernelIdentity  = (double *) malloc(kernelSize * kernelSize * sizeof(double));
  double *kernelInterpolated = (double *) malloc(kernelSize * kernelSize * sizeof(double));

  computeIdentityKernel(kernelIdentity, kernelSize);

  int margin = (kernelSize - 1) / 2;

  for (int h = 0; h < height; h++)
  {
    for (int w = 0; w < width; w++ )
    {

      /* Only convolute area within rectangle */
      if (w < minX || w > maxX || h < minY || h > maxY)
      {
        for (int c = 0; c < components; c++)
        {
          outPixel[c] = inPixel[c];
        }
      }

      /* Convolute area within rectangle */
      else
      {

        int dx = abs(areaCenter - (w - minX));
        int dy = abs(areaCenter - (h - minY));
        double v = sqrt(dx * dx + dy * dy);

        v *= 2;              // radius to diameter
        v *= 1.0 / areaSize; // fit
        v = 1 - v;           // inverse
        v = v > 0 ? v : 0;   // clamp

        interpolate(
          v,                       // weight
          kernelIdentity,          // a
          kernel,                  // b
          kernelInterpolated,      // c
          kernelSize               // size
        );

        /* Mix each component separately */
        for (int component = 0; component < components; component++)
        {
          int sum = 0;

          for (int col = 0, i = 0; col < kernelSize; col++)
          {
            for (int row = 0; row < kernelSize; row++)
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

              const uint8_t *samplePixel = inPixel
                + (col - margin) * width * components
                + (row - margin) * components;

              sum += kernelInterpolated[i] * samplePixel[component];

              i++;
            }
          }

          outPixel[component] = sum;
        }
      }

    inPixel += components;
    outPixel += components;

    }
  }
  
  free(kernelIdentity);
  free(kernelInterpolated);

  return true;
}

double computeGaussian(double x, double y, double sigma, double mean)
{
    return exp(-0.5 * (pow((x - mean) / sigma, 2.0)
                     + pow((y - mean) / sigma, 2.0)))
                / (2 * M_PI * sigma * sigma);
}

double computeKernel(double *out,
                     const int W,
                     const double sigma)
{
    double mean = W / 2,
           sum = 0.0;

    for (int x = 0, i = 0; x < W; ++x)
        for (int y = 0; y < W; ++y) {
            out[i] = computeGaussian(x, y, sigma, mean);
            sum += out[i];
            i++;
        }

    return sum;
}


int normalise(double *out,
              const double sum,
              const int width,
              const int height)
{
  for (int h = 0, i = 0; h < height; h++)
  {
    for (int w = 0; w < width; w++)
    {
      out[i] /= sum;
      i++;
    }
  }

  return 0;
}


double computeSum(const double *in,
                  const int width,
                  const int height)
{
  double sum = 0;
  for (int h = 0, i = 0; h < height; h++)
  {
    for (int w = 0; w < width; w++)
    {
      sum += in[i];
      i++;
    }
  }

  return sum;
}


int computeIdentityKernel(double *out, const int W)
{
  assert(W % 2 == 1);

  int center = W * 0.5;
  for (int x = 0, i = 0; x < W; ++x)
    for (int y = 0; y < W; ++y) {
        out[i] = x == center && y == center ? 1 : 0;
        i++;
    }

  return 0;
}

void printKernel(const double *kernel, const int W)
{
    for (int x = 0, i = 0; x < W; x++)
    {
        for (int y = 0; y < W; y++)
        {
            printf("%.2f ", kernel[i]);
            i++;
        }

        printf("\n");
    }
}

void computeRamp(double *out, const int W)
{
    int center = W / 2;
    for (int col = 0, i = 0; col < W; col++)
    {
        for (int row = 0; row < W; row++)
        {
            int deltaX = abs(center - col);
            int deltaY = abs(center - row);
            double v = sqrt(deltaX * deltaX + deltaY * deltaY);

            v *= 2;              // radius to diameter
            v *= 1.0 / W;        // fit
            v = 1 - v;           // inverse
            v = v > 0 ? v : 0;   // clamp

            out[i] = v;

            i++;
        }
    }
}

int interpolate(const double weight,
                const double *a,
                const double *b,
                double *c,
                const int W)
{
    for (int x = 0, i = 0; x < W; x++)
    {
        for (int y = 0; y < W; y++)
        {
            c[i] = a[i] * (1 - weight) + b[i] * weight;
            i++;
        }
    }
    return 0;
}

int scale(const int factor,
          const uint8_t *in,
          uint8_t *out,
          const int width,
          const int height,
          const int components)
{
    if (factor == 1)
    {
        memcpy(out, in, width * height * components * sizeof(uint8_t));
        return 0;
    }

    for (int row = 0, sourceIndex = 0, targetIndex = 0; row < height; row++)
    {
        for (int y = 0; y < factor; y++)
        {
            for (int col = 0; col < width; col++)
            {
                for (int x = 0; x < factor; x++)
                {
                    for (int c = 0; c < components; c++)
                    {
                        out[targetIndex] = in[sourceIndex + c];
                        targetIndex++;
                    }
                }

                sourceIndex += components;
            }

            if (y < factor - 1)
            {
                // rewind
                sourceIndex -= width * components;
            }
        }
    }

    return 0;
}

int fit(double *in,
        const int width,
        const int height,
        const double oldMin, // e.g. 0
        const double oldMax, // e.g. 1
        const double newMin, // e.g. 0
        const double newMax) // e.g. 255
{
    for (int x = 0, i = 0; x < width; ++x) 
        for (int y = 0; y < height; ++y)
        {
            in[i] += newMin - oldMin;
            in[i] *= newMax / oldMax;
            i++;
        }

    return 0;
}
