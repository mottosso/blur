#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "args.h"


bool parseArgs(int argc, char **argv, char **filenameIn, char **filenameOut, int *x, int *y, int *size, int *iterations, int *kernelSize, int *interactive)
{

  int c;
  while ((c = getopt(argc, argv, "x:y:s:n:k:i:o:")) != -1)
    switch (c)
    {
      case 'x':
        /* X-position of effect; 0 means left (default) */
        *x = atoi(optarg);
        if (*x < 1)
        {
          printf("X-position must be positive.\n");
          return false;
        }
        break;
      case 'y':
        /* Y-position of effect; 0 means top (default) */
        *y = atoi(optarg);
        if (*y < 1)
        {
          printf("Y-position must be positive.\n");
          return false;
        }
        break;
      case 's':
        /* Size of effect; 0 means the full image (default) */
        *size = atoi(optarg);
        if (*size < 1)
        {
          printf("Size must be positive.\n");
          return false;
        }
        break;
      case 'i':
        /* Interactive mode */
        *interactive = true;
        break;
      case 'n':
        /* Number of iterations; defaults to 1 */
        *iterations = atoi(optarg);
        if (*iterations < 1)
        {
          printf("Number of iterations must be positive.\n");
          return false;
        }
        break;
      case 'k':
        /* Size of kernel; 3 or 5 */
        *kernelSize = atoi(optarg);
        if (*kernelSize % 2 != 1)
        {
          printf("Kernel size (%i) must be odd-numbered.\n", *kernelSize);
          return false;
        }
        break;
      case 'o':
        *filenameOut = optarg;
        break;
      default:
        return false;
    }

  if (argc - optind != 1)
  {
    printf("Usage: ./blur [-o] [-x] [-y] [-s] [-k] [-i] input\n");
    return false;
  }

  *filenameIn = argv[optind];

  if (strcasecmp(strrchr(*filenameIn, '.'), ".png") != 0)
  {
      printf("Input must be PNG.\n");
      return false;
  }

  return true;
}

int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}
