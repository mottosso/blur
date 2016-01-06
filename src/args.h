#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

bool parseArgs(int argc,
               char **argv,
               char **filenameIn,
               char **filenameOut,
               int *x,
               int *y,
               int *size,
               int *iterations,
               int *kernelSize,
               int *interactive);
