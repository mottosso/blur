#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/resource.h>  // rusage

/**
 * Returns number of seconds between b and a.
 */
double calculate(const struct rusage* b, const struct rusage* a);

/** 2d convolution filter
 *
 * @param width         Width in pixels of /p data
 * @param height        Height in pixels of /p data
 * @param components    Number of components of each pixel; i.e. RGB = 3
 * @param in            Pointer to where pixels are read
 * @param out           Pointer to where pixels is written
 * @param kernel        Matrix which to apply to pixel data
 * @param kernelSize    Width and height of kernel
 *
 * Pixeldata is in the stb_image.h format; i.e. *y scanlines of *x pixels,
 * with each pixel consisting of N interleaved 8-bit components; the first
 * pixel pointed to is top-left-most in the image.
 *
 * Returns true if successful, false otherwise
 *
 * Reference:
 *  - http://setosa.io/ev/image-kernels/
 *  - https://docs.gimp.org/en/plug-in-convmatrix.html
 *
 */
bool convolve(int width,        // width of image
              int height,       // height of image
              int components,   // number of components/channels
              uint8_t *in,      // pointer to the start of incoming image
              uint8_t *out,     // pointer to the start of outgoing image
              int *kernel,      // kernel used for convolution
              int kernelSize);  // size of (square) kernel

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
int extract(uint8_t *in,
            uint8_t *out,
            int x,
            int y,
            int sourceWidth,
            int sourceHeight,
            int targetWidth,
            int targetHeight,
            int components);

/** Inject trimmed chunk into bigger chunk
 *
 *    ___       ______________
 *   |_|_|     |    ___       |
 *   |_|_| ------->|_|_|      |
 *             |   |_|_|      |
 *             |              |
 *             |              |
 *             |______________|
 *
 * Inserts a chunk back into source image.
 *
 */
int integrate(uint8_t *in,
              uint8_t *out,
              int x,
              int y,
              int sourceWidth,
              int sourceHeight,
              int targetWidth,
              int targetHeight,
              int components);