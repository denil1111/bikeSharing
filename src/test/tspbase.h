/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef TSP_BASE_H
#define TSP_BASE_H

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "commenhead.h"
#include "debug.h"

// Full Graph
#include <lemon/full_graph.h>
#include <lemon/math.h>
#include <lemon/maps.h>
#include <lemon/random.h>
#include <lemon/dim2.h>
#include <lemon/christofides_tsp.h>

using namespace lemon;
using namespace std;

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

struct point{
	int a;
	int b;
};

class TspBase
{

public:

	const int K = STATION_CAPACITY;
	const int Q = VEHICLE_CAPACITY;
	int _stationNum;
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	FullGraph *g;
	DoubleEdgeMap *cost;
	vector<int> _path;

public:

	vector<point> _point;
	void getRandomPoints();
	void getPoints();

	vector<vector<int> > _cost;
	void getRandomCost();
	void getCost();

	vector<int> _stationDemand;
	void getRandomDemand();
	void getDemand();

	void randomData();

	int _tspSum;

public:

	TspBase(int num);
	~TspBase();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	void printPoints();
	void printDemand();
	void printCost();
	void printTSPtour();

	// check
	void checkRandomDemand();

};// class BikeSharing

#endif
