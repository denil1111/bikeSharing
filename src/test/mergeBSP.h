/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef MERGE_BSP_H
#define MERGE_BSP_H

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <time.h>
#include <math.h>

// Matching
#include <lemon/matching.h>
#include <lemon/smart_graph.h>
#include <lemon/concepts/graph.h>
#include <lemon/concepts/maps.h>
#include <lemon/lgf_reader.h>
#include <lemon/math.h>

#include "test_tools.h"
#include "commenhead.h"

// Full Graph
#include <lemon/full_graph.h>
#include <lemon/math.h>
#include <lemon/maps.h>
#include <lemon/random.h>
#include <lemon/dim2.h>
#include <lemon/christofides_tsp.h>

#include "supernode.h"
#include "debug.h"

using namespace lemon;
using namespace std;

// used for . Min cost perfect matching convert to maximum matching.

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

struct point{
	int a;
	int b;
};

class MergeBSP
{

private:

	// 正站点访问标记,如果访问过，就放进这个vector中。如果下次又轮到这个点，说明无解
	// used for UncapacitatedBSP:
	vector<int> _positiveStationVisiteFlag;
	int _startStation;

	int _stationNum;
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	double _sum;
	int _finalTSPSum;
	int _finalSum;
	int _startStationId;
	FullGraph *g;
	DoubleEdgeMap *cost;
	vector<int> _path;
	

	const int K = STATION_CAPACITY;
	const int Q = VEHICLE_CAPACITY;

	int _superNodeNumber;
	int _zeroSuperNodeNumberInFront;
	int _superNodeNumber_PIECE_P;
	int _superNodeNumber_PIECE_N;
	int _superNodeNumber_PIECE_0;
	vector<SuperNode> _superNodeVector_PIECE_P;
	vector<SuperNode> _superNodeVector_PIECE_N;
	vector<SuperNode> _superNodeVector_PIECE_0;
	vector< vector<MinCostOfTwoSuperNode> > _minCostAmongSuperNode;


public:
	// get data
	vector<point> _point;
	void getRandomPoints();
	void getPoints();

	vector<vector<int> > _cost;
	void getCost();
	void getRandomCost();

	vector<int> _stationDemand;
	void getRandomDemand();
	void getDemand();


public:
	vector<int> _finalPaht;
	bool isExistNotVisitedPositiveStation();
	bool isAPositiveStation(int number);
	int  getStartStation();

	void printResult();

	MergeBSP(int num);

	
	
	
	template <typename TSP>
	void getTspTour(const std::string &alg_name);
	void getSuperNodePieces();
	void initMinCostAmongSuperNode();
	void calculateMinCostOfTwoSuperNode(int first, int second);
	void calculateMinCostAmongSuperNode();
	string getLGF();
	void machingSuperNode();
	int  getStartStationId();
	void getZeroPath(int currentnumberofzeropiece);
	void getZeroPathInFront();
	void getPath();

	//
	void getZeroPathReverse(int currentnumberofzeropiece);
	void getZeroPathBehindPositive(int &currentnumberofzeropiece);
	void getZeroPathBehindNegative(int &currentnumberofzeropiece);
	void getCompletePath();

	int  getTSPSum();
	int  getFinalSum();

	void printRandomPoints();
	void printCost();
	void printRandomDemand();
	void printTSPtour();
	void printSuperNodeInformation();
	void printFinalPath();

	void randomData();
	void run();

};// class BikeSharing

#endif
