#include <stdio.h>
#include <string.h>
#include <stdint.h>


/** 2d convolution filter
 *
 * Pixeldata is in the stb_image.h format; i.e. *y scanlines of *x pixels,
 * with each pixel consisting of N interleaved 8-bit components; the first
 * pixel pointed to is top-left-most in the image.
 *
 * @param width         Width in pixels of /p data
 * @param height        Height in pixels of /p data
 * @param components    Number of components of each pixel; i.e. RGB = 3
 * @param in            Pointer to where pixels are read
 * @param out           Pointer to where pixels is written
 * @param kernel        Matrix which to apply to pixel data
 * @param kernelSize    Width and height of kernel
 * @param mask          Multiply the effect
 * @returns             true if successful
 *
 * Reference:
 *  - http://setosa.io/ev/image-kernels/
 *  - https://docs.gimp.org/en/plug-in-convmatrix.html
 *  - http://www.pixelstech.net/article/1353768112-Gaussian-Blur-Algorithm
 *  - http://www.imagemagick.org/Usage/convolve/
 */
bool convolve(const int width,        // width of image
              const int height,       // height of image
              const int minX,         //
              const int minY,         //
              const int maxX,         // maximum width from which to sample the kernel
              const int maxY,         // maximum height ..
              const int components,   // number of components/channels
              const uint8_t *in,      // pointer to the start of incoming image
              uint8_t *out,           // pointer to the start of outgoing image
              const double *kernel,   // kernel used for convolution
              const int kernelSize);  // size of (square) kernel


/** Trim image
 *  ______________
 * |    ___       |        
 * |   |_|_|      |     ___ 
 * |   |_|_|---------->|_|_|
 * |              |    |_|_|
 * |              |
 * |______________|
 *
 * Returns 0 for success, 1 if source material can't
 * accommodate for target dimensions.
 *  
 */
int extract(const uint8_t *in,
            uint8_t *out,
            const int x,
            const int y,
            const int sourceWidth,
            const int sourceHeight,
            const int targetWidth,
            const int targetHeight,
            const int components);


/** Inject trimmed chunk into bigger chunk
 *
 *    ___       ______________
 *   |_|_|     |    ___       |
 *   |_|_|-------->|_|_|      |
 *             |   |_|_|      |
 *             |              |
 *             |              |
 *             |______________|
 *
 * Inserts a chunk back into source image.
 *
 */
int integrate(const uint8_t *in,
              uint8_t *out,
              const int x,
              const int y,
              const int sourceWidth,
              const int sourceHeight,
              const int targetWidth,
              const int targetHeight,
              const int components);


/** Gaussian formula
 */
double computeGaussian(const double x,
                       const double y,
                       const double sigma,
                       const double mean);

/** Compute linear array of gaussian values
 *
 * @param W           dimensions of array
 * @param normalised  whether or not to normalise the array
 *
 */
double computeKernel(double *out,
                     const int W,
                     const double sigma);


int computeIdentityKernel(double *out, const int W);


void printKernel(const double *kernel, const int W);


/** Compute radial ramp, between 0-1
 *  _________
 * |   ___   |
 * |  / _ \  |
 * | | |_| | |
 * |  \___/  |
 * |_________|
 *
 * @param out    pointer to where pixels will get written
 * @param W      dimensions of array
 * @param gamma  gamma of ramp
 */
void computeRamp(double *out, const int W);


/** Interpolate between two linear matrices
 *
 * @param factor  value between 0-1, 0 means A and 1 means B
 * @param a       source A
 * @param b       source B
 * @param c       output
 * @param W       dimension of arrays
 *
 */
int interpolate(const double weight,
                const double *a,
                const double *b,
                double *c,
                const int W);

int scale(const int factor,
          const uint8_t *in,
          uint8_t *out,
          const int width,
          const int height,
          const int components);

/** Fit array between min/max
 *
 * @param  in      input array of pixels
 * @param  width   width of array
 * @param  height  height of array
 * @param  oldMin  previous minimum value in array
 * @param  oldMax  previous maximum value in array
 * @param  newMin  new minimum value in array
 * @param  newMax  new maximum value in array
 * @returns        0 for success, non-0 otherwise
 */
int fit(double *in,
        const int width,
        const int height,
        const double oldMin,  // e.g. 0
        const double oldMax,  // e.g. 1
        const double newMin,  // e.g. 0
        const double newMax); // e.g. 255


int normalise(double *out,
              const double sum,
              const int width,
              const int height);


double computeSum(const double *in,
                  const int width,
                  const int height);