/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef CAPACITATED_BSP_H
#define CAPACITATED_BSP_H

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

class CapacitatedBSP :public TspBase
{

private:

	int _startFromWhichPiece;
	int _startStationCapacitatedBSP;
	int _sumCapacitatedBSP;
	int _superNodeNumber; // positive plus negative plus last one zero super node
	int _zeroSuperNodeNumberInFront;
	//  opposite direction:
	int _zeroSuperNodeNumberInEnd;
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

	CapacitatedBSP(int num);
	~CapacitatedBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	void getSuperNodePieces(int number);
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

	// used for opposite direction:
	void getPositivePathReverse(int currentnumberofzeropiece, vector<int> &path);
	void getNegativePathReverse(int currentnumberofzeropiece, vector<int> &path);
	void getZeroPathReverse(int currentnumberofzeropiece, vector<int> &path);
	void getZeroPathInBehind(vector<int> &path);
	void getPathBeginPositiveReverse();
	void getPathBeginNegativeReverse();

	// printf something:
	void printSuperNodeInformation();
	void printFinalPath();

};

#endif
