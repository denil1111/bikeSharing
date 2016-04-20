#include "commenhead.h"
#include <cstdio>

long MM = 10000000;
int M=100000;
//int STATION_CAPACITY=10;
//int VEHICLE_CAPACITY=30;
int POINT_RANGE=100;

char* itoa(long num, char* s, int len)
{
  sprintf(s,"%d", num);
  return s;
}
