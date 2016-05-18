#include "bspbase.h"
#include "uncapacitatedBSP.h"
#include "capacitated.h"
#include "ktimescapacitatedBSP.h"
#include "noZeroCapacitatedBSP.h"
#include "tspbase.h"
#include <iostream>

using namespace std;

int main()
{
  int stationnum = 0;
  int stationcapacity = 5;
  int vehiclecapacity = 10;
  cin >> stationnum;

  TspBase tsp(stationnum, stationcapacity, vehiclecapacity);
  tsp.randomData();
  tsp.data();
  UncapacitatedBSP unbsp(tsp);
  unbsp.run();

  KTimesCapacitatedBSP kbsp(tsp);
  kbsp.run();

  return 0;
}
