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
public:

	vector<vector<StationidAndDemand> > _pathSet;
	vector<StationidAndDemand> _minCostPath;
	vector<StationidAndDemand> _minMediumCostPath;
	int _minSum;
	int _startStationCapacitatedBSP;

	void run();

private:

	int _startFromWhichPiece;
	// positive plus negative plus last one zero super node
	int _superNodeNumber; 
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
	int _startPoint;

public:

	KTimesCapacitatedBSP(int num);
	KTimesCapacitatedBSP(int num, int x, int y);
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

	// used for find a valid start point:
	int  getStartStationCapacitated();
	void deleteRepeatStationPoint();
	void revertPath();

	// printf something:
	void printSuperNodeInformation();
	void printTspPath();
	void printTempPath(vector<StationidAndDemand> temp);
	void printFinalPath();

	// check
	bool  checkSum();

};

#endif
