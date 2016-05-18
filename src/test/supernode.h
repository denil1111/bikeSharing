
#ifndef SUPER_NODE_H
#define SUPER_NODE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <time.h>

// Matching
#include <lemon/matching.h>
#include <lemon/smart_graph.h>
#include <lemon/concepts/graph.h>
#include <lemon/concepts/maps.h>
#include <lemon/lgf_reader.h>
#include <lemon/math.h>

using namespace lemon;
using namespace std;

// piece type:
#ifndef SP
#define SP

#define PIECE_0		0
#define PIECE_P		1
#define PIECE_N		-1
#define PIECE_NULL	250

#endif

struct MinCostOfTwoSuperNode{
	int cost;
	vector<int>::iterator firstNodeIt;
	vector<int>::iterator secondNodeIt;
	int first;
	int second;
};

struct StationidAndDemand{
	int stationId;
	int stationDemand;
	int isCutPoint;
	int formerPosition;
	int latterPosition;
};

class SuperNode{

public:

	int _numberOfZeroPieceBehind;
	int _numberOfZeroPieceFront;
	vector<int> _zeroPieceIndexFront;
	vector<int> _zeroPieceIndexBehind;


	vector<int> _zeroCostPoint;
	int _zeroCostPointNumber;
	int _nowPieceTypeFlag;
	bool _isFirstTime;

	//int _lastNodeDemand;
	//// matching number to another super node
	int _matchingNumber;
	// mincostmatchingnumber
	int _minCostMatchingNumber;
	// piece bound
	int _excessNumber;
	// number
	int _nodeNumberInSuperNode;
	// super node ID
	int _superNodeId;
	// start iterator in _path
	vector<int>::iterator _startIt;
	// end iterator in _path
	vector<int>::iterator _endIt;
	// piece type:( 0: 0-piece, 1:p-piece, -1:n-piece
	int _pieceTypeFlag;
	// sum of piece
	bool flag;

public:

	// used for calculate min cost between positive node and negative node:
	int _positiveEnd;
	int _positiveReverseEnd;
	int _positiveNext;
	int _positiveReverseNext;
	int _negativeEnd;
	int _negativeReverseEnd;
	int _negativeNext;
	int _negativeReverseNext;


	SuperNode(int excessnumber);

	vector<StationidAndDemand> _stationidAndDemand;

	void setMinCostMatchingNumber(int number);
	void setsuperNodeNumber(int supernodeId);
	void setMatchingNumber(int number);
	int& getNumberOfZeroPieceBehind();
	//  opposite direction:
	int& getNumberOfZeroPieceFront();
	void setNumberOfZeroPieceFront(int number);
	int getMatchingNumber();
	int getSuperNodeId();
	vector<int>::iterator getStartIt();
	vector<int>::iterator getEndIt();
	int getNodeNumberInSuperNode();
	int getPieceTypeFlag();

	int getASuperNode(vector<int> &path, vector<int> &demand, int startnode, vector<int>::iterator &currentIt, int surplusdemand, int &stopflag, int stationnum);
	int getASuperNodeNoZero(vector<int> &path, vector<int> &demand, int startnode, vector<int>::iterator &currentIt, int surplusdemand, int &stopflag, int stationnum);
};

#endif