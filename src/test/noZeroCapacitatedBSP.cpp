#include "noZeroCapacitatedBSP.h"



NoZeroCapacitatedBSP::NoZeroCapacitatedBSP(TspBase &tspbase):BspBase(tspbase){
	capacityFlag = true;
}

NoZeroCapacitatedBSP::~NoZeroCapacitatedBSP(){

}

//void NoZeroCapacitatedBSP::getPath(){
//	if (_startFromWhichPiece == PIECE_P){
//		cout << "_startFromWhichPiece:PIECE_P" << endl;
//		getPathBeginPositive();
//		getPathBeginNegativeReverse();
//	}
//	else{
//		cout << "_startFromWhichPiece:PIECE_N" << endl;
//		getPathBeginNegative();
//		getPathBeginPositiveReverse();
//	}
//}

void NoZeroCapacitatedBSP::calculateMinCostOfTwoSuperNode(int positivesupernode, int negativesupernode){
	// get four points:
	int positivelastid = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.size() - 1].stationId;
	int negativelastid = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.size() - 1].stationId;
	int negativefirstid = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[0].stationId;
	int nextpositivefirstid;
	if (positivesupernode == _superNodeNumber_PIECE_P - 1){
		nextpositivefirstid = _superNodeVector_PIECE_P[0]._stationidAndDemand[0].stationId;
	}
	else{
		nextpositivefirstid = _superNodeVector_PIECE_P[positivesupernode + 1]._stationidAndDemand[0].stationId;
	}

	FullGraph::Node unode, vnode, xnode, ynode;
	unode = (*_tspBase.g)(positivelastid);
	xnode = (*_tspBase.g)(nextpositivefirstid);
	vnode = (*_tspBase.g)(negativelastid);
	ynode = (*_tspBase.g)(negativefirstid);

	int costuv, costxy;
	if (unode == vnode){
		costuv = 0;
	}
	else{
		costuv = (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)];
	}
	if (xnode == ynode){
		costxy = 0;
	}
	else{
		costxy = (*_tspBase.cost)[(*_tspBase.g).edge(xnode, ynode)];
	}

	_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = costxy + costuv;

	return;
}

void NoZeroCapacitatedBSP::getOrderPath(vector<SuperNode> firstPIECE, vector<SuperNode> secondPIECE){
	vector<StationidAndDemand> tempVector;

	if (_superNodeNumber >= 2){
		for (int first = 0; first < _superNodeNumber / 2; ++first){
			// positive station:
			getSuperPath(firstPIECE[first], tempVector);
			// matching negative station:
			int second = firstPIECE[first].getMatchingNumber();
			getSuperPathReverse(secondPIECE[second], tempVector);
		}
	}
	else{
		for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_0[0]._stationidAndDemand.begin(); it < _superNodeVector_PIECE_0[0]._stationidAndDemand.end(); ++it){
			pushbackStationidAndDemand(tempVector, *it);
		}
	}

	getStartStation(tempVector, _minCostPath, _minSum);

	PRINTFTempPath

}


void NoZeroCapacitatedBSP::getReversePath(vector<SuperNode> firstPIECE, vector<SuperNode> secondPIECE){
	vector<StationidAndDemand> tempVector;

	if (_superNodeNumber >= 2){
		for (int first = _superNodeNumber / 2 - 1; first >= 0; --first){
			// positive station:
			getSuperPathReverse(firstPIECE[first], tempVector);
			// matching negative station:
			int second = _superNodeVector_PIECE_P[first].getMatchingNumber();
			getSuperPath(secondPIECE[second], tempVector);
		}
	}
	else{
		for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_0[0]._stationidAndDemand.begin(); it < _superNodeVector_PIECE_0[0]._stationidAndDemand.end(); ++it){
			pushbackStationidAndDemand(tempVector, *it);
		}
	}

	getStartStation(tempVector, _minCostPath, _minSum);

	PRINTFTempPath

}

void NoZeroCapacitatedBSP::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			calculateMinCostOfTwoSuperNode(i, j);
		}
	}
}

void NoZeroCapacitatedBSP::run(){

	//_tspBase.data();

	cout << endl << "NoZeroCapacitatedBSP:" << endl;

	for (int i = 0; i < _tspBase._stationNum; i++){
		getSuperNodePiecesNoZero(i);
		calculateMinCostAmongSuperNode();
		machingSuperNode();
		getPath();
		//cout << "Get a path " << i << endl << endl;
	}

	PRINTFFinalPath
	PRINTFSuperNodeInformation

	cout << "noZeroCapacitatedBSP sum cost:" << _minSum << endl;
	
}

