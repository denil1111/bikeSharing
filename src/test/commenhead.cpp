#include "commenhead.h"
#include <cstdio>

long MM = 10000000;
int M = 100000;
int POINT_RANGE = 100;
int POSITIVE_POINT_RANGE = 20;
int LEFT_POINT_RANGE = 30;
int RIGHT_POINT_RANGE = 50;

char* itoa(long num, char* s, int len)
{
  sprintf(s,"%d", num);
  return s;
}
