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

	point _depot;
	vector<int> _stationDemand;
	vector<point> _point;
	void randomData();
	void data();

public:

	const int K = STATION_CAPACITY;
	const int Q = VEHICLE_CAPACITY;
	int _stationNum;
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	FullGraph *g;
	DoubleEdgeMap *cost;
	vector<int> _path;
	vector<vector<int> > _cost;
	int _tspSum;

public:

	TspBase(int num);
	TspBase(int num, int x, int y);
	~TspBase();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	void setDepot(int x, int y);
	void getRandomPoints();
	void getPoints();
	void getRandomCost();
	void getCost();
	void getRandomDemand();
	void getDemand();

	// print something
	void printPoints();
	void printDemand();
	void printCost();
	void printTSPtour();

	// check
	void checkDemand();

};// class BikeSharing

#endif
