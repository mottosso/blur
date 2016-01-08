#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <libgen.h>
#include <stdio.h>

#include "args.h"


bool parseArgs(int argc,
               char **argv,
               char **filenameIn,
               char **filenameOut,
               int *x,
               int *y,
               int *size,
               int *iterations,
               int *kernelSize,
               double *radius,
               int *interactive)
{

  int c;
  while ((c = getopt(argc, argv, "x:y:s:n:k:i:o:r:")) != -1)
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
        /* Size of kernel */
        *kernelSize = atoi(optarg);
        if (*kernelSize % 2 != 1)
        {
          printf("Kernel size (%i) must be odd-numbered.\n", *kernelSize);
          return false;
        }
        break;
      case 'r':
        /* Radius of effect */
        *radius = atof(optarg) / 2;
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

  *filenameIn = realpath(argv[optind], NULL);

  if (*filenameIn == NULL)
  {
    bool hasAccess = access(argv[optind], F_OK) == 0;
    bool canRead = access(argv[optind], R_OK) == 0;

    if (!hasAccess)
    {
      printf("The file \"%s\" does not exist.\n", argv[optind]);
      return false;
    }

    else if (!canRead)
    {
      printf("The file \"%s\" cannot be read.\n", argv[optind]);
      return false;
    }
    else
    {
      printf("The file \"%s\" could not be opened.\n", argv[optind]);
      return false;
    }
  }

  /* Generate a filename */
  char *filenameOutDyn;
  if (*filenameOut == NULL)
  {
      /* No path given, append suffix to input */
      const char *base = basename(*filenameIn);
      const char *suffix = "_out.png";
      filenameOutDyn = (char *) calloc(strlen(base)
                           + sizeof(suffix)
                           + 1, sizeof(char));
      strncpy(filenameOutDyn, base, strlen(base));
      const char *dot = strrchr(filenameOutDyn, '.');
      filenameOutDyn[strlen(filenameOutDyn) - strlen(dot)] = '\0';
      strcat(filenameOutDyn, suffix);
  }
  else
  {
      /* Path given, copy value into new array (so we can free it) */
      filenameOutDyn = (char *) calloc(strlen(*filenameOut) + 1, sizeof(char));
      strncpy(filenameOutDyn, *filenameOut, strlen(*filenameOut));
  }

  *filenameOut = filenameOutDyn;

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
