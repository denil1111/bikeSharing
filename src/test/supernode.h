
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
	// 	symmetric. first represent positive pieces.second represent negative pieces. Whatever clockwise or anticlockwise.
	int first;
	int second;
};

struct StationidAndDemand{
	int stationId;
	int stationDemand;
	int pieceType;
	int pieceId;
	// used for tryToMeetPositive and tryToMeetNegative in bspbase.cpp.
	int formerPosition;
	int latterPosition;
};

class SuperNode{

public:
	// used for save all of the station information about this super node.
	vector<StationidAndDemand> _stationidAndDemand;
	// matching id to another super node
	int _matchingNumber;
	// piece bound.
	int _excessNumber;
	// piece type:( 0: 0-piece, 1:p-piece, -1:n-piece
	int _pieceTypeFlag;

	// clockwise dirction:
	int _numberOfZeroPieceBehind;
	// clockwise dirction : zero piece id.
	vector<int> _zeroPieceIndexBehind;
	// anti-clockwise dirction:
	int _numberOfZeroPieceFront;
	// anti-clockwise dirction : zero piece id.
	vector<int> _zeroPieceIndexFront;

	// used for calculate min cost between positive node and negative node:
	// in ktimescapacitatedBSP.cpp and capacitated.cpp.
	int _positiveEnd;
	int _positiveReverseEnd;
	int _positiveNext;
	int _positiveReverseNext;
	int _negativeEnd;
	int _negativeReverseEnd;
	int _negativeNext;
	int _negativeReverseNext;

public:

	SuperNode(int excessnumber);
	~SuperNode();

	// path:Tsp tour.
	// currentIt:current iterator in path.
	// demand:all station demand.
	// surplusdemand:the previous super node surplus demand which because of segmentation.
	// startnode:whole partitioning start station id.
	// stopflag:whether patition finish.
	// stationnum:whole station which demand dosen't equal zero. heheda.
	int getASuperNode(vector<int> &path, vector<int>::iterator &currentIt, vector<int> &demand, int surplusdemand, int startnode, int &stopflag, int stationnum);
	int getASuperNodeNoZero(vector<int> &path, vector<int>::iterator &currentIt, vector<int> &demand, int surplusdemand, int startnode, int &stopflag, int stationnum);
};

#endif