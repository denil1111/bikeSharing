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

// Baidu map point:(a,b)
struct point{
	double a;
	double b;
};

class TspBase
{

public:

	int STATION_CAPACITY;
	int VEHICLE_CAPACITY;

	
	// dosen't include depot. All stations include some stations which demand is zero. Corresponding to Baidu map station.
	int _allStationNum;
	// dosen't include depot. Exclude the station which demand is zero.
	int _stationNum;
	// bike warehouse.
	point _depot;
	// 
	vector<point> _point;
	// Corresponding to the Baidu map. 
	vector<vector<double> > _cost;
	
	// dosen't include depot. Include station which damand is zero. Corresponding to Baidu map station.
	vector<int> _allStationDemand;
	// dosen't include depot. Exclude the station which demand is zero.
	vector<int> _stationDemand;
	// station id. _mapPartToAll[i] represent location station i correspond to Baidu map _mapPartToAll[i].
	vector<int> _mapPartToAll;
	// _mapAllToPart[i] == -1 represent station i's demand equal zero.
	vector<int> _mapAllToPart;

	
	void randomDataSplitRegion();
	void randomData();
	void inputData();
	void data();
	void dataGeneral();

public:

	
	
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	FullGraph *g;
	DoubleEdgeMap *cost;
	vector<int> _path;
	
	int _tspSum;

	Debug debug;
public:
	TspBase();
	TspBase(int num, int stationcapacity, int vehiclecapacity);
	TspBase(int num, int stationcapacity, int vehiclecapacity, double x, double y);
	~TspBase();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);
	template <typename TSP>
	void getTspTourGeneral(const std::string &alg_name);

	void setDepot(double x, double y);

	void getRandomPointsSplitRegion();
	void getRandomPoints();
	void getInputPoints();
	void getPoints();
	
	void getRandomCost();
	void getInputCost();
	void getCost();
	
	void getRandomDemand();
	void getInputDemand();
	void getDemand();

	// print something
	void printPoints();
	void printDemand();
	void printCost();
	void printTSPtour();

};

#endif
