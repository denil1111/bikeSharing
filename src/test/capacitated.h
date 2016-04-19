/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef CAPACITATED_BSP_H
#define CAPACITATED_BSP_H_BSP_H

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

class CapacitatedBSP
{

public:

	void run();
	void runRandom();

	TspBase _tspbase;
	vector<vector<StationidAndDemand> > _pathSet;
	vector<StationidAndDemand> _minCostPath;
	vector<StationidAndDemand> _minMediumCostPath;
	int _minSum;
	int _startStationCapacitatedBSP;
	point _depot;

private:

	int _startFromWhichPiece;
	int _sumCapacitatedBSP;
	// positive plus negative plus last one zero super node
	int _superNodeNumber; 
	//  opposite direction:
	int _zeroSuperNodeNumberInEnd;
	int _zeroSuperNodeNumberInFront;
	int _superNodeNumber_PIECE_P;
	int _superNodeNumber_PIECE_N;
	int _superNodeNumber_PIECE_0;
	vector<SuperNode> _superNodeVector_PIECE_P;
	vector<SuperNode> _superNodeVector_PIECE_N;
	vector<SuperNode> _superNodeVector_PIECE_0;
	vector<vector<MinCostOfTwoSuperNode> > _minCostAmongSuperNode;

	int _startPoint;

public:
	
	CapacitatedBSP(TspBase &tspbase);
	~CapacitatedBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	void getSuperNodePieces(int number);
	void initMinCostAmongSuperNode();
	void initSuperNode();
	void calculateMinCostOfTwoSuperNode(int first, int second);
	void calculateMinCostAmongSuperNode();
	string getLGF();
	void machingSuperNode();
	int  getStartStationCapacitatedBSP();

	void pushbackStationidAndDemand(vector<StationidAndDemand> &tempVector, StationidAndDemand temp);
	void getPositivePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getNegativePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getZeroPath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getZeroPathInFront(vector<StationidAndDemand> &tempVector);
	void getPathBeginPositive();
	void getPathBeginNegative();
	void getPath();
	int  getFinalSum(vector<StationidAndDemand> temp);

	// used for opposite direction:
	void getPositivePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getNegativePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getZeroPathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getZeroPathInBehind(vector<StationidAndDemand> &tempVector);
	void getPathBeginPositiveReverse();
	void getPathBeginNegativeReverse();

	int  getStartStationCapacitated(vector<StationidAndDemand> &mincostpath);
	void deleteRepeatStationPoint(vector<StationidAndDemand> &mincostpath);
	void revertPath(vector<StationidAndDemand> &mincostpath);
	void tryToMeetPositive(vector<StationidAndDemand> &mincostpath);
	void tryToMeetNegative(vector<StationidAndDemand> &mincostpath);
	void mapPath();

	// printf something:
	void printSuperNodeInformation();
	void printTspPath();
	void printTempPath(vector<StationidAndDemand> temp);
	void printFinalPath();

	// check
	bool checkSum();

	// new 
	void beginPositive(int positivesupernode, int negativesupernode);
	void beginPositiveReverse(int positivesupernode, int negativesupernode);
	void beginNegative(int positivesupernode, int negativesupernode);
	void beginNegativeReverse(int positivesupernode, int negativesupernode);
};

#endif
