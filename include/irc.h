#ifndef IRC_H
#define IRC_H

#define MAX(a, b) ((a > b) ? a : b)

int xsafe(int err, int res, char *str);
void *xpsafe(void *err, void *res, char *str);

#define XPSAFE(err, res, str) (xpsafe(err, res, str))
#define XSAFE(err, res, str) (xsafe(err, res, str))

#define ISOTIMESTRSIZE 25
void time2iso(char *str);

#endif