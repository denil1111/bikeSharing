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

// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void NoZeroCapacitatedBSP::getTspTour(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	TSP alg(*_tspBase.g, *_tspBase.cost);
	_tspBase._tspSum = alg.run();

	std::vector<Node> vec;
	alg.tourNodes(std::back_inserter(vec));
	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		FullGraph::Node node = *it;
		_tspBase._path.push_back((*_tspBase.g).index(node));
	}

	PRINTFTspPath
}




//void NoZeroCapacitatedBSP::initMinCostAmongSuperNode(){
//	_minCostAmongSuperNode.clear();
//	for (int i = 0; i < _superNodeNumber / 2; i++){
//		vector<MinCostOfTwoSuperNode> m;
//		for (int j = 0; j < _superNodeNumber / 2; j++){
//			MinCostOfTwoSuperNode minCostOfTwoSuperNode;
//			minCostOfTwoSuperNode.cost = M;
//			minCostOfTwoSuperNode.firstNodeIt = _tspBase._path.end();
//			minCostOfTwoSuperNode.secondNodeIt = _tspBase._path.end();
//			m.push_back(minCostOfTwoSuperNode);
//		}
//		_minCostAmongSuperNode.push_back(m);
//	}
//}
//
//void NoZeroCapacitatedBSP::initSuperNode(){
//	_startFromWhichPiece = PIECE_0;
//	_startStationCapacitatedBSP = -1;
//	_superNodeNumber = 0;
//	_superNodeNumber_PIECE_P = 0;
//	_superNodeNumber_PIECE_N = 0;
//	_superNodeNumber_PIECE_0 = 0;
//	_zeroSuperNodeNumberInFront = 0;
//	_superNodeVector_PIECE_P.clear();
//	_superNodeVector_PIECE_N.clear();
//	_superNodeVector_PIECE_0.clear();
//	_minCostAmongSuperNode.clear();
//	initMinCostAmongSuperNode();
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
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);
	tryToMeetPositive(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

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
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);

	cout << endl;
	int sum = 0;
	for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
		sum += (*it).stationDemand;
		cout << (*it).stationId << "(" << (*it).stationDemand << ") ";
		if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
			cout << "positive cao!!!!!!!!!!!!" << endl;
			break;
		}
	}
	cout << endl;
	tryToMeetPositive(tempVector);
	sum = 0;
	for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
		sum += (*it).stationDemand;
		cout << (*it).stationId << "(" << (*it).stationDemand << ") ";

		if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
			cout << "positive after cao!!!!!!!!!!!!" << endl;
			//break;
		}
	}
	cout << endl;
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

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
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);
	tryToMeetPositive(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

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
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);

	cout << endl;
	int sum = 0;
	for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
		sum += (*it).stationDemand;
		cout <<(*it).stationId << "(" << (*it).stationDemand << ") ";
		if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
			cout << "negative cao!!!!!!!!!!!!" << endl;
			break;
		}
	}
	cout << endl;
	tryToMeetNegative(tempVector);
	sum = 0;
	for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
		sum += (*it).stationDemand;
		cout << (*it).stationId << "(" << (*it).stationDemand << ") ";
		if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
			cout << "negative after cao!!!!!!!!!!!!" << endl;
			//break;
		}
	}
	cout << endl;


	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			_minCostPath.push_back(*it);
		}
	}

	PRINTFTempPath

}

//int NoZeroCapacitatedBSP::getFinalSum(vector<StationidAndDemand> temp){
//	FullGraph::Node unode, vnode;
//	int i = 0;
//	int tempsum = 0;
//	vector<StationidAndDemand>::iterator it = temp.begin();
//
//	unode = (*_tspBase.g)((*it++).stationId);
//	for (; it < temp.end() - 1; ++it){
//		vnode = (*_tspBase.g)((*it).stationId);
//		if (vnode != unode){
//			tempsum += (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)];
//		}
//		unode = vnode;
//	}
//
//	return tempsum;
//}
//
//bool  NoZeroCapacitatedBSP::checkSum(){
//	int sum = 0;
//	for (int i = 0; i < _minCostPath.size(); i++){
//		sum += _minCostPath[i].stationDemand;
//	}
//	if (sum != 0){
//		cout << "Final sum != 0!!!!!!!" << endl;
//		return false;
//	}
//	else{
//		return true;
//	}
//}
//
//int NoZeroCapacitatedBSP::getStartStationCapacitatedBSP(){
//	return _startStationCapacitatedBSP;
//}
//
//int NoZeroCapacitatedBSP::getStartStationCapacitated(vector<StationidAndDemand> &mincostpath){
//	vector<StationidAndDemand>::const_iterator it = mincostpath.begin();
//	vector<StationidAndDemand>::const_iterator itt = mincostpath.begin();
//	_startPoint = 0;
//	if (checkSum()){
//		//if (true){
//		while (true){
//			//cout << "*";
//			// find a station which demond is positive :
//			if ((*it).stationDemand >= 0){
//				_startStationCapacitatedBSP = (*it).stationId;
//				int tempSum = (*it).stationDemand;
//				int tempNum = 1;
//
//				while (tempSum >= 0 && tempNum < mincostpath.size()){
//					++it;
//					++tempNum;
//					if (it == mincostpath.end()){
//						it = mincostpath.begin();
//					}
//					tempSum += (*it).stationDemand;
//
//				}
//
//				if (tempNum == mincostpath.size()){
//					//cout << endl;
//					return _startStationCapacitatedBSP;
//				}
//			}
//
//			if (++itt == mincostpath.end()){
//				itt = mincostpath.begin();
//			}
//			it = itt;
//			_startPoint++;
//		}// while
//	}// if
//
//	return -1;
//}
//
//void NoZeroCapacitatedBSP::revertPath(vector<StationidAndDemand> &mincostpath){
//	for (int i = 0; i < _startPoint; i++){
//		StationidAndDemand temp;
//		temp.isCutPoint = (*mincostpath.begin()).isCutPoint;
//		temp.stationId = (*mincostpath.begin()).stationId;
//		temp.stationDemand = (*mincostpath.begin()).stationDemand;
//		mincostpath.erase(mincostpath.begin());
//		mincostpath.push_back(temp);
//	}
//}




//void NoZeroCapacitatedBSP::mapPath(){
//	for (vector<StationidAndDemand>::iterator it = _minCostPath.begin(); it < _minCostPath.end(); it++){
//		(*it).stationId = _tspBase._mapPartToAll[(*it).stationId];
//	}
//}
//
//void NoZeroCapacitatedBSP::deleteRepeatStationPoint(vector<StationidAndDemand> &mincostpath){
//	vector<StationidAndDemand>::iterator it = mincostpath.begin();
//	vector<StationidAndDemand> temp;
//	temp.push_back(*it);
//	for (it++; it < mincostpath.end(); ++it){
//		if ((*it).stationId == temp[temp.size() - 1].stationId){
//			temp[temp.size() - 1].stationDemand += (*it).stationDemand;
//		}
//		else{
//			temp.push_back(*it);
//		}
//	}
//
//	mincostpath.clear();
//	for (vector<StationidAndDemand>::iterator it = temp.begin(); it < temp.end(); ++it){
//		StationidAndDemand temp;
//		temp.isCutPoint = (*it).isCutPoint;
//		temp.stationId = (*it).stationId;
//		temp.stationDemand = (*it).stationDemand;
//		mincostpath.push_back(temp);
//	}
//}
//


void NoZeroCapacitatedBSP::runRandom(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	totaltime0 = finish - sum;

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

	PRINTFFinalPath

	cout << "noZeroCapacitatedBSP sum cost:" << _minSum << endl;
	//cout << "Mininum sum cost:" << _minSum << endl;
	//cout << "StartStation:" << _startStationCapacitatedBSP << endl;
	//cout << "length:" << _minCostPath.size() << endl;
	//cout << "Path set size:" << _pathSet.size() << endl;

}

void NoZeroCapacitatedBSP::run(){

	_tspBase.data();

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	totaltime0 = finish - sum;

	cout << endl << "NoZeroCapacitatedBSP:" << endl;

	//for (int i = 0; i < VEHICLE_CAPACITY / 2; i++){
	int i = 0;
	initSuperNode();
	getSuperNodePiecesNoZero(i);
	calculateMinCostAmongSuperNode();
	//machingSuperNode();
	//getPath();
	//cout << "Get a path " << i << endl << endl;
	//}
	mapPath();

	PRINTFFinalPath

	cout << "noZeroCapacitatedBSP sum cost:" << _minSum << endl;
	//cout << "Mininum sum cost:" << _minSum << endl;
	//cout << "StartStation:" << _startStationCapacitatedBSP << endl;
	//cout << "length:" << _minMediumCostPath.size() << endl;
	//cout << "Path set size:" << _pathSet.size() << endl;

}

