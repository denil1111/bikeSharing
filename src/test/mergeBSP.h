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
#include <fstream>

// Matching
#include <lemon/matching.h>
#include <lemon/smart_graph.h>
#include <lemon/concepts/graph.h>
#include <lemon/concepts/maps.h>
#include <lemon/lgf_reader.h>
#include <lemon/math.h>

#include "test_tools.h"

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
#define M 1000000
#define STATION_CAPACITY 10
#define VEHICLE_CAPACITY 100
#define POINT_RANGE 100

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

class MergeBSP
{

private:

	ofstream _outFile;

	// 正站点访问标记,如果访问过，就放进这个vector中。如果下次又轮到这个点，说明无解
	// used for UncapacitatedBSP:
	vector<int> _positiveStationVisiteFlag;
	int _startStation;

	int _stationNum;
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	double _sum;
	int _finalTSPSum;
	int _finalSum;
	vector<int> _stationDemand;
	int _startStationId;
	FullGraph *g;
	DoubleEdgeMap *cost;
	vector<int> _path;
	vector<int> _finalPaht;

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

	bool isExistNotVisitedPositiveStation();
	bool isAPositiveStation(int number);
	int  getStartStation();

	void printResult();

	MergeBSP(int num);

	void getRandomDemand();
	void getRandomPoints();
	void getCost();
	void getRandomCost();
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
	int  getTSPSum();
	int  getFinalSum();

	void printRandomPoints();
	void printCost();
	void printRandomDemand();
	void printTSPtour();
	void printSuperNodeInformation();
	void printFinalPaht();

	void runRandom();


	void outFileRandomPoints();
	void outFileRandomDemand();
	void outFileCost();

	void outFileFinalPaht();

};// class BikeSharing

#endif
