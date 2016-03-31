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

#include "tspbase.h"

// Matching
#include <lemon/matching.h>
#include <lemon/smart_graph.h>
#include <lemon/concepts/graph.h>
#include <lemon/concepts/maps.h>
#include <lemon/lgf_reader.h>
#include <lemon/math.h>

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

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

class MergeBSP :public TspBase
{

private:

	// used for UncapacitatedBSP:
	vector<int> _positiveStationVisiteFlag;
	int _startStationUncapacitatedBSP;

	// user for CapacitatedBSP:
	int _startStationCapacitatedBSP;
	int _sumCapacitatedBSP;
	int _superNodeNumber; // positive plus negative plus last one zero super node
	int _zeroSuperNodeNumberInFront;
	int _superNodeNumber_PIECE_P;
	int _superNodeNumber_PIECE_N;
	int _superNodeNumber_PIECE_0;
	vector<SuperNode> _superNodeVector_PIECE_P;
	vector<SuperNode> _superNodeVector_PIECE_N;
	vector<SuperNode> _superNodeVector_PIECE_0;
	vector<vector<MinCostOfTwoSuperNode> > _minCostAmongSuperNode;

public:

	vector<int> _finalPath;

	void run();

public:
	
	MergeBSP(int num);
	~MergeBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	// used for Uncapacitated BSP:
	bool isExistNotVisitedPositiveStation();
	bool isAPositiveStation(int number);
	int  getStartStation();

	// used for Capacitated BSP:
	void getSuperNodePieces();
	void initMinCostAmongSuperNode();
	void calculateMinCostOfTwoSuperNode(int first, int second);
	void calculateMinCostAmongSuperNode();
	string getLGF();
	void machingSuperNode();
	int  getStartStationCapacitatedBSP();
	void getZeroPath(int currentnumberofzeropiece);
	void getZeroPathInFront();
	void getPath();
	int  getFinalSum();

	void printSuperNodeInformation();
	void printFinalPath();

};// class BikeSharing

#endif
