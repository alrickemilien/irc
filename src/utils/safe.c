#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "irc.h"

int xsafe(int err, int res, char *str)
{
  if (res == err) {
    fprintf(stderr, "%s: %s\n", str, strerror(errno));
    exit(1);
  }
  return (res);
}

void *xpsafe(void *err, void *res, char *str)
{
  if (res == err) {
    fprintf(stderr, "%s: %s\n", str, strerror(errno));
    exit(1);
  }
  return (res);
}
