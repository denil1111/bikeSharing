/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef K_TIMES_CAPACITATED_BSP_H
#define K_TIMES_CAPACITATED_BSP_H

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

class KTimesCapacitatedBSP :public TspBase
{

private:
	int _startFromWhichPiece;
	int _startStationCapacitatedBSP;
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

	vector<vector<int> > _pathSet;
	vector<int> _minCostPath;
	int _minSum;
	void run();

public:

	KTimesCapacitatedBSP(int num);
	~KTimesCapacitatedBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	void getSuperNodePieces(int number);
	void initSuperNode();
	void initMinCostAmongSuperNode();
	void calculateMinCostOfTwoSuperNode(int first, int second);
	void calculateMinCostAmongSuperNode();
	string getLGF();
	void machingSuperNode();
	int  getStartStationCapacitatedBSP();
	void getPositivePath(int currentnumberofzeropiece, vector<int> &path);
	void getNegativePath(int currentnumberofzeropiece, vector<int> &path);
	void getZeroPath(int currentnumberofzeropiece, vector<int> &path);
	void getZeroPathInFront(vector<int> &path);
	void getPathBeginNegative();
	void getPathBeginPositive();
	void getPath();
	int  getFinalSum(vector<int> &path);

	// printf something:
	void printSuperNodeInformation();
	void printTspPath();
	void printFinalPath();

};

#endif
