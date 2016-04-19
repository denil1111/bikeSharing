/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef UNCAPACITATED_BSP_H
#define UNCAPACITATED_BSP_H

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "tspbase.h"
#include "commenhead.h"

// Full Graph
#include <lemon/full_graph.h>
#include <lemon/math.h>
#include <lemon/maps.h>
#include <lemon/random.h>
#include <lemon/dim2.h>
#include <lemon/christofides_tsp.h>

#include "debug.h"

using namespace lemon;
using namespace std;

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

class UncapacitatedBSP
{

public:

	TspBase _tspBase;
	vector<int> _minCostPath;

private:

	vector<int> _positiveStationVisiteFlag;
	int _startStationUncapacitatedBSP;
	int _startPoint;

public:
	UncapacitatedBSP(TspBase &tspbase);
	~UncapacitatedBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	bool isExistNotVisitedPositiveStation();
	bool isAPositiveStation(int number);
	int  getStartStation();
	void revertPath();
	void run();

	void printFinalPath();
};

#endif
