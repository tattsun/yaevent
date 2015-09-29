#include "misc.h"
#include <stdio.h>
#include <stdlib.h>

void yev_die(const char* msg)
{
  perror(msg);
  exit(1);
}

void yev_die_noerror(const char *msg)
{
  fprintf(stderr, "%s", msg);
  exit(1);
}
