/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef BSPBASE_BSP_H
#define BSPBASE_BSP_H

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
#include <lemon/opt2_tsp.h>

#include "supernode.h"
#include "debug.h"

using namespace lemon;
using namespace std;

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

class BspBase
{

public:

	//void run();
	//void runRandom();

	TspBase _tspBase;
	vector<vector<StationidAndDemand> > _pathSet;
	vector<StationidAndDemand> _minCostPath;
	int _minSum;
	int _startStationCapacitatedBSP;
	point _depot;

public:

	int _startPoint;

	// positive plus negative plus last one zero super node
	int _superNodeNumber;
	int _startFromWhichPiece;
	int _zeroSuperNodeNumberInEnd;
	int _zeroSuperNodeNumberInFront;
	int _superNodeNumber_PIECE_P;
	int _superNodeNumber_PIECE_N;
	int _superNodeNumber_PIECE_0;
	vector<SuperNode> _superNodeVector_PIECE_P;
	vector<SuperNode> _superNodeVector_PIECE_N;
	vector<SuperNode> _superNodeVector_PIECE_0;

	vector<vector<MinCostOfTwoSuperNode> > _minCostAmongSuperNode;
	virtual void run()=0;

public:
	BspBase();
	BspBase(TspBase &tspbase);
	~BspBase();

	void initMinCostAmongSuperNode();
	void initSuperNode();
	void getSuperNodePieces(int number);
	void getSuperNodePiecesNoZero(int number);
	virtual void calculateMinCostOfTwoSuperNode(int first, int second);
	virtual void calculateMinCostAmongSuperNode();
	string getLGF();
	void machingSuperNode();

	void pushbackStationidAndDemand(vector<StationidAndDemand> &tempVector, StationidAndDemand temp);
	void getPositivePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getPositivePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getNegativePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getNegativePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getZeroPath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getZeroPathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector);
	void getZeroPathInFront(vector<StationidAndDemand> &tempVector);
	void getZeroPathInBehind(vector<StationidAndDemand> &tempVector);
	virtual void getPathBeginPositive();
	virtual void getPathBeginPositiveReverse();
	virtual void getPathBeginNegative();
	virtual void getPathBeginNegativeReverse();

	int  getStartStation(vector<StationidAndDemand> &mincostpath);
	void deleteRepeatStationPoint(vector<StationidAndDemand> &mincostpath);
	void revertPath(vector<StationidAndDemand> &mincostpath);
	void tryToMeetPositive(vector<StationidAndDemand> &mincostpath);
	void tryToMeetNegative(vector<StationidAndDemand> &mincostpath);
	void mapPath();
	void getPath();
	int  getFinalSum(vector<StationidAndDemand> temp);

	// check:
	bool checkSum();

	// printf something:
	void printSuperNodeInformation();
	void printTspPath();
	void printTempPath(vector<StationidAndDemand> temp);
	void printFinalPath();
};

#endif
