/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*
*/

#ifndef UNCAPACITATED_BSP_H
#define UNCAPACITATED_BSP_H

#include <lemon/full_graph.h>
#include <lemon/math.h>
#include <lemon/maps.h>
#include <lemon/random.h>
#include <lemon/dim2.h>

#include <lemon/christofides_tsp.h>
#include <iostream>
#include <time.h>
#include "debug.h"

using namespace lemon;
using namespace std;

#define STATION_CAPACITY 10
#define VEHICLE_CAPACITY 30

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

class UncapacitatedBSP
{

private:

	int _stationNum;
	vector<int> _path;
	double _sum;
	vector<int> _stationDemand;
	FullGraph *g;
	DoubleEdgeMap *cost;
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	// 正站点访问标记,如果访问过，就放进这个vector中。如果下次又轮到这个点，说明无解
	vector<int> _positiveStationVisiteFlag;

	const int K = STATION_CAPACITY;
	const int Q = VEHICLE_CAPACITY;
	
	int _startStation;

public:

	UncapacitatedBSP(int num);

	void getRandomDemand();
	void getRandomPoints();
	void getCost();

	// Get a Random TSP sequence
	// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	// Get a TSP sequence
	// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	template <typename TSP>
	void getTspSequence(const std::string &alg_name);

	bool isExistNotVisitedPositiveStation();
	bool isAPositiveStation(int number);
	int getStartStation();

	void printRandomPoints();
	void printRandomDemand();
	void printCost();
	void printTSPtour();
	void printResult();

	void runRandom();

};// class BikeSharing

#endif
