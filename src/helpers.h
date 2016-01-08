#include <sys/resource.h>  // rusage

/**
 * Returns number of seconds between b and a.
 */
double calculate(const struct rusage* b, const struct rusage* a);
