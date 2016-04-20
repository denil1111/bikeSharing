#include "noZeroCapacitatedBSP.h"



NoZeroCapacitatedBSP::NoZeroCapacitatedBSP(TspBase &tspbase){
	_tspBase = tspbase;

	_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_minSum = MM;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
}

NoZeroCapacitatedBSP::~NoZeroCapacitatedBSP(){

}

//// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
//template <typename TSP>
//void NoZeroCapacitatedBSP::getTspTour(const std::string &alg_name) {
//	GRAPH_TYPEDEFS(FullGraph);
//
//	TSP alg(*_tspBase.g, *_tspBase.cost);
//	_tspBase._tspSum = alg.run();
//
//	std::vector<Node> vec;
//	alg.tourNodes(std::back_inserter(vec));
//	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
//	{
//		FullGraph::Node node = *it;
//		_tspBase._path.push_back((*_tspBase.g).index(node));
//	}
//
//	PRINTFTspPath
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

void NoZeroCapacitatedBSP::getPathBeginPositiveReverse(){
	vector<StationidAndDemand> tempVector;

	if (_superNodeNumber >= 2){
		for (int positivesupernode = _superNodeNumber / 2 - 1; positivesupernode >= 0; --positivesupernode){
			// positive station:
			getPositivePathReverse(positivesupernode, tempVector);
			// matching negative station:
			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();
			getNegativePath(negativesupernode, tempVector);
		}
	}
	else{
		for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_0[0]._stationidAndDemand.begin(); it < _superNodeVector_PIECE_0[0]._stationidAndDemand.end(); ++it){
			pushbackStationidAndDemand(tempVector, *it);
		}
	}

	deleteRepeatStationPoint(tempVector);
	getStartStation(tempVector);
	revertPath(tempVector);
	//tryToMeetPositive(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	int depotA = 0, depotB = 0;
	StationidAndDemand start = tempVector[0];
	StationidAndDemand end = tempVector[tempVector.size() - 1];
	depotA = _tspBase._cost[_tspBase._mapPartToAll[start.stationId]][_tspBase._allStationNum - 1];
	depotB = _tspBase._cost[_tspBase._mapPartToAll[end.stationId]][_tspBase._allStationNum - 1];
	if ((tempsum = getFinalSum(tempVector) + depotA + depotB) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			_minCostPath.push_back(*it);
		}
	}

	PRINTFTempPath

}

// new:
void NoZeroCapacitatedBSP::getPathBeginPositive(){
	vector<StationidAndDemand> tempVector;

	if (_superNodeNumber >= 2){
		for (int positivesupernode = 0; positivesupernode < _superNodeNumber / 2; ++positivesupernode){
			// positive station:
			getPositivePath(positivesupernode, tempVector);
			// matching negative station:
			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();
			getNegativePathReverse(negativesupernode, tempVector);
		}
	}
	else{
		for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_0[0]._stationidAndDemand.begin(); it < _superNodeVector_PIECE_0[0]._stationidAndDemand.end(); ++it){
			pushbackStationidAndDemand(tempVector, *it);
		}
	}

	deleteRepeatStationPoint(tempVector);
	getStartStation(tempVector);
	revertPath(tempVector);

	//cout << endl;
	//int sum = 0;
	//for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
	//	sum += (*it).stationDemand;
	//	cout << (*it).stationId << "(" << (*it).stationDemand << ") ";
	//	if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
	//		cout << "positive cao!!!!!!!!!!!!" << endl;
	//		break;
	//	}
	//}
	//cout << endl;
	//tryToMeetPositive(tempVector);
	//sum = 0;
	//for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
	//	sum += (*it).stationDemand;
	//	cout << (*it).stationId << "(" << (*it).stationDemand << ") ";

	//	if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
	//		cout << "positive after cao!!!!!!!!!!!!" << endl;
	//		//break;
	//	}
	//}
	//cout << endl;
	_pathSet.push_back(tempVector);


	int tempsum = 0;
	int depotA = 0, depotB = 0;
	StationidAndDemand start = tempVector[0];
	StationidAndDemand end = tempVector[tempVector.size() - 1];
	depotA = _tspBase._cost[_tspBase._mapPartToAll[start.stationId]][_tspBase._allStationNum - 1];
	depotB = _tspBase._cost[_tspBase._mapPartToAll[end.stationId]][_tspBase._allStationNum - 1];
	if ((tempsum = getFinalSum(tempVector) + depotA + depotB) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			_minCostPath.push_back(*it);
		}
	}

	PRINTFTempPath

}

void NoZeroCapacitatedBSP::getPathBeginNegativeReverse(){
	vector<StationidAndDemand> tempVector;

	if (_superNodeNumber >= 2){
		for (int negativesupernode = _superNodeNumber / 2 - 1; negativesupernode >= 0 ; --negativesupernode){
			// positive station:
			getNegativePathReverse(negativesupernode, tempVector);
			// matching negative station:
			int positivesupernode = _superNodeVector_PIECE_N[negativesupernode].getMatchingNumber();
			getPositivePath(positivesupernode, tempVector);
		}
	}
	else{
		for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_0[0]._stationidAndDemand.begin(); it < _superNodeVector_PIECE_0[0]._stationidAndDemand.end(); ++it){
			pushbackStationidAndDemand(tempVector, *it);
		}
	}

	deleteRepeatStationPoint(tempVector);
	getStartStation(tempVector);
	revertPath(tempVector);
	//tryToMeetPositive(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	int depotA = 0, depotB = 0;
	StationidAndDemand start = tempVector[0];
	StationidAndDemand end = tempVector[tempVector.size() - 1];
	depotA = _tspBase._cost[_tspBase._mapPartToAll[start.stationId]][_tspBase._allStationNum - 1];
	depotB = _tspBase._cost[_tspBase._mapPartToAll[end.stationId]][_tspBase._allStationNum - 1];
	if ((tempsum = getFinalSum(tempVector) + depotA + depotB) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			_minCostPath.push_back(*it);
		}
	}

	PRINTFTempPath

}

void NoZeroCapacitatedBSP::getPathBeginNegative(){
	vector<StationidAndDemand> tempVector;

	if (_superNodeNumber >= 2){
		for (int negativesupernode = 0; negativesupernode < _superNodeNumber / 2; ++negativesupernode){
			// positive station:
			getNegativePath(negativesupernode, tempVector);
			// matching negative station:
			int positivesupernode = _superNodeVector_PIECE_N[negativesupernode].getMatchingNumber();
			getPositivePathReverse(positivesupernode, tempVector);
		}
	}
	else{
		for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_0[0]._stationidAndDemand.begin(); it < _superNodeVector_PIECE_0[0]._stationidAndDemand.end(); ++it){
			pushbackStationidAndDemand(tempVector, *it);
		}
	}

	deleteRepeatStationPoint(tempVector);
	getStartStation(tempVector);
	revertPath(tempVector);

	//cout << endl;
	//int sum = 0;
	//for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
	//	sum += (*it).stationDemand;
	//	cout <<(*it).stationId << "(" << (*it).stationDemand << ") ";
	//	if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
	//		cout << "negative cao!!!!!!!!!!!!" << endl;
	//		break;
	//	}
	//}
	//cout << endl;
//	tryToMeetNegative(tempVector);
	//sum = 0;
	//for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
	//	sum += (*it).stationDemand;
	//	cout << (*it).stationId << "(" << (*it).stationDemand << ") ";
	//	if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
	//		cout << "negative after cao!!!!!!!!!!!!" << endl;
	//		//break;
	//	}
	//}
	//cout << endl;


	_pathSet.push_back(tempVector);

	int tempsum = 0;
	int depotA = 0, depotB = 0;
	StationidAndDemand start = tempVector[0];
	StationidAndDemand end = tempVector[tempVector.size() - 1];
	depotA = _tspBase._cost[_tspBase._mapPartToAll[start.stationId]][_tspBase._allStationNum - 1];
	depotB = _tspBase._cost[_tspBase._mapPartToAll[end.stationId]][_tspBase._allStationNum - 1];
	if ((tempsum = getFinalSum(tempVector) + depotA + depotB) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			_minCostPath.push_back(*it);
		}
	}

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

//	_tspBase.data();

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();


	cout << endl << "NoZeroCapacitatedBSP:" << endl;

	//for (int i = 0; i < VEHICLE_CAPACITY / 2; i++){
	int i = 0;
	initSuperNode();
	getSuperNodePiecesNoZero(i);
	calculateMinCostAmongSuperNode();
	machingSuperNode();
	getPath();
	//cout << "Get a path " << i << endl << endl;
	//}
	mapPath();

	PRINTFFinalPath

	cout << "noZeroCapacitatedBSP sum cost:" << _minSum << endl;

}

