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
#include <lemon/opt2_tsp.h>

using namespace lemon;
using namespace std;

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

struct point{
	double a;
	double b;
};

class TspBase
{

public:

	point _depot;

	int _stationNum;
	int _allStationNum;
	vector<int> _stationDemand;
	vector<int> _allStationDemand;
	vector<int> _mapPartToAll;
	vector<int> _mapAllToPart;

	vector<point> _point;
	vector<vector<double> > _cost;
	void randomData();
	void data();

public:

	int STATION_CAPACITY;
	int VEHICLE_CAPACITY;
	
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	FullGraph *g;
	DoubleEdgeMap *cost;
	vector<int> _path;
	
	int _tspSum;

public:
	TspBase();
	TspBase(int num, int stationcapacity, int vehiclecapacity);
	TspBase(int num, int stationcapacity, int vehiclecapacity, double x, double y);
	~TspBase();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	void setDepot(double x, double y);
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
