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

void NoZeroCapacitatedBSP::getSuperNodePieces(int number){
	int startstation;
	
	// used for forward direction to record last one super node:
	int lastsupernodeflag = PIECE_NULL;

	vector<int>::iterator it = _tspBase._path.begin();
	for (int i = 0; i < number; i++){
		if (++it == _tspBase._path.end()){
			it = _tspBase._path.begin();
		}
	}
	startstation = *it;

	//cout << "Start station id:" << startstation << " "<<_tspBase._stationDemand[*it] <<endl << endl;
	int surplusdemand = 0;

	int flag = true;
	int stopflag = 0;
	while (stopflag < _tspBase._stationNum){

		SuperNode newpiece(_tspBase.VEHICLE_CAPACITY / 2);
		surplusdemand = newpiece.getASuperNodeNoZero(_tspBase._path, _tspBase._stationDemand, startstation, it, surplusdemand, stopflag, _tspBase._stationNum);
		//surplusdemand = newpiece.getASuperNode(_tspBase._path, _tspBase._stationDemand, startstation, it, surplusdemand, stopflag, _tspBase._stationNum);

		if (newpiece.getPieceTypeFlag() == PIECE_P){
			if (flag == true){
				flag = false;
				_startFromWhichPiece = PIECE_P;
			}

			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;

			_superNodeVector_PIECE_P.push_back(newpiece);
			lastsupernodeflag = PIECE_P;

		}
		else if (newpiece.getPieceTypeFlag() == PIECE_N){
			if (flag == true){
				flag = false;
				_startFromWhichPiece = PIECE_N;
			}

			_superNodeNumber_PIECE_N++;
			++_superNodeNumber;

			_superNodeVector_PIECE_N.push_back(newpiece);
			lastsupernodeflag = PIECE_N;

		}
		else{
			_superNodeVector_PIECE_0.push_back(newpiece);
			_superNodeNumber_PIECE_0++;

			//cout << "_startFromWhichPiece = " << _startFromWhichPiece << endl;
			if (_startFromWhichPiece == PIECE_P){
				for (vector<StationidAndDemand>::iterator it = newpiece._stationidAndDemand.end() - 1;
					it > newpiece._stationidAndDemand.begin(); --it){
					_superNodeVector_PIECE_P[0]._stationidAndDemand.insert(_superNodeVector_PIECE_P[0]._stationidAndDemand.begin(), *it);
				}
				if (newpiece._stationidAndDemand.size() != 0){
					_superNodeVector_PIECE_P[0]._stationidAndDemand.insert(_superNodeVector_PIECE_P[0]._stationidAndDemand.begin(), newpiece._stationidAndDemand[0]);
				}
			}
			else if (_startFromWhichPiece == PIECE_N){
				for (vector<StationidAndDemand>::iterator it = newpiece._stationidAndDemand.end() - 1;
					it > newpiece._stationidAndDemand.begin(); --it){
					_superNodeVector_PIECE_N[0]._stationidAndDemand.insert(_superNodeVector_PIECE_N[0]._stationidAndDemand.begin(), *it);
				}
				if (newpiece._stationidAndDemand.size() != 0){
					_superNodeVector_PIECE_N[0]._stationidAndDemand.insert(_superNodeVector_PIECE_N[0]._stationidAndDemand.begin(), newpiece._stationidAndDemand[0]);

				}
			}
		}
	}

}


void NoZeroCapacitatedBSP::initMinCostAmongSuperNode(){
	_minCostAmongSuperNode.clear();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		vector<MinCostOfTwoSuperNode> m;
		for (int j = 0; j < _superNodeNumber / 2; j++){
			MinCostOfTwoSuperNode minCostOfTwoSuperNode;
			minCostOfTwoSuperNode.cost = M;
			minCostOfTwoSuperNode.firstNodeIt = _tspBase._path.end();
			minCostOfTwoSuperNode.secondNodeIt = _tspBase._path.end();
			m.push_back(minCostOfTwoSuperNode);
		}
		_minCostAmongSuperNode.push_back(m);
	}
}

void NoZeroCapacitatedBSP::initSuperNode(){
	_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
	_superNodeVector_PIECE_P.clear();
	_superNodeVector_PIECE_N.clear();
	_superNodeVector_PIECE_0.clear();
	_minCostAmongSuperNode.clear();
	initMinCostAmongSuperNode();
}

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

void NoZeroCapacitatedBSP::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			calculateMinCostOfTwoSuperNode(i, j);
		}
	}
}

string NoZeroCapacitatedBSP::getLGF(){
	/* Instance:
	const std::string lgf = {
	"@nodes\n"
	"label\n"
	"0\n"
	"1\n"
	"2\n"
	"3\n"
	"4\n"
	"5\n"
	"6\n"
	"7\n"
	"@edges\n"
	"     label  weight\n"
	"7 4  0      984\n"
	"0 7  1      73\n"
	"7 1  2      204\n"
	"2 3  3      583\n"
	"2 7  4      565\n"
	"2 1  5      582\n"
	"0 4  6      551\n"
	"2 5  7      385\n"
	"1 5  8      561\n"
	"5 3  9      484\n"
	"7 5  10     904\n"
	"3 6  11     47\n"
	"7 6  12     888\n"
	"3 0  13     747\n"
	"6 1  14     310\n"
	};
	*/

	char *tempstring;
	tempstring = new char[10];
	string lgf;
	lgf += {"@nodes\n"
		"label\n"};

	int number = _superNodeNumber;
	if (_superNodeNumber % 2 != 0){
		number = _superNodeNumber - 1;
	}
	for (int i = 0; i < number; i++){
		itoa(i, tempstring, 10);
		lgf += tempstring;
		lgf += "\n";
	}
	lgf += "@edges\n";
	lgf += "     label  weight\n";
	int label = 0;
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){

			itoa(i, tempstring, 10);
			lgf += tempstring;
			lgf += " ";

			itoa(j + _superNodeNumber / 2, tempstring, 10);
			lgf += tempstring;
			lgf += "  ";

			itoa(label++, tempstring, 10);
			lgf += tempstring;
			lgf += "     ";

			itoa(M - _minCostAmongSuperNode[i][j].cost, tempstring, 10);
			lgf += tempstring;
			lgf += "\n";
		}
	}

	return lgf;
}

void NoZeroCapacitatedBSP::machingSuperNode(){
	GRAPH_TYPEDEFS(SmartGraph);

	string lgf = getLGF();

	SmartGraph graph;
	SmartGraph::EdgeMap<int> weight(graph);

	istringstream lgfs(lgf);
	graphReader(graph, lgfs).edgeMap("weight", weight).run();

	bool perfect = true;

	MaxWeightedPerfectMatching<SmartGraph> mwpm(graph, weight);
	mwpm.init();
	bool result = mwpm.start();

	// graph.id(node);
	for (NodeIt n(graph); n != INVALID; ++n) {
		if ((mwpm.matchingMap())[n] != INVALID){
			int number_N = graph.id(n);
			int number_P = graph.id((mwpm.mate(n)));
			if (number_N >= _superNodeNumber / 2){
				_superNodeVector_PIECE_N[number_N - _superNodeNumber / 2].setMatchingNumber(graph.id((mwpm.mate(n))));
				_superNodeVector_PIECE_P[number_P].setMatchingNumber(number_N - _superNodeNumber / 2);
			}
		}
	}

	PRINTFSuperNode

}

void NoZeroCapacitatedBSP::pushbackStationidAndDemand(vector<StationidAndDemand> &tempVector, StationidAndDemand temp){
	int num = tempVector.size();
	if (num == 0){
		tempVector.push_back(temp);
	}
	else{
		tempVector.push_back(temp);
	}
}

void NoZeroCapacitatedBSP::getPositivePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void NoZeroCapacitatedBSP::getPositivePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void NoZeroCapacitatedBSP::getNegativePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void NoZeroCapacitatedBSP::getNegativePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void NoZeroCapacitatedBSP::getZeroPathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void NoZeroCapacitatedBSP::getZeroPath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void NoZeroCapacitatedBSP::getZeroPathInBehind(vector<StationidAndDemand> &tempVector){
	int zeronum = _superNodeVector_PIECE_0.size();
	int i = zeronum - 1;
	for (int j = 0; j < _zeroSuperNodeNumberInEnd; j++, i--){
		getZeroPathReverse(i, tempVector);
	}
}

void NoZeroCapacitatedBSP::getZeroPathInFront(vector<StationidAndDemand> &tempVector){
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPath(i, tempVector);
	}
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
	tryToMeetPositive(tempVector);
	tryToMeetNegative(tempVector);
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
	tryToMeetNegative(tempVector);
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

void NoZeroCapacitatedBSP::getPath(){
	if (_startFromWhichPiece == PIECE_P){
		//cout << "_startFromWhichPiece:PIECE_P" << endl;
		getPathBeginPositive();
		getPathBeginNegativeReverse();
	}
	else{
		//cout << "_startFromWhichPiece:PIECE_N" << endl;
		getPathBeginNegative();
		getPathBeginPositiveReverse();
	}
}

int NoZeroCapacitatedBSP::getFinalSum(vector<StationidAndDemand> temp){
	FullGraph::Node unode, vnode;
	int i = 0;
	int tempsum = 0;
	vector<StationidAndDemand>::iterator it = temp.begin();

	unode = (*_tspBase.g)((*it++).stationId);
	for (; it < temp.end() - 1; ++it){
		vnode = (*_tspBase.g)((*it).stationId);
		if (vnode != unode){
			tempsum += (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)];
		}
		unode = vnode;
	}

	return tempsum;
}

bool  NoZeroCapacitatedBSP::checkSum(){
	int sum = 0;
	for (int i = 0; i < _minCostPath.size(); i++){
		sum += _minCostPath[i].stationDemand;
	}
	if (sum != 0){
		cout << "Final sum != 0!!!!!!!" << endl;
		return false;
	}
	else{
		return true;
	}
}

int NoZeroCapacitatedBSP::getStartStationCapacitatedBSP(){
	return _startStationCapacitatedBSP;
}

int NoZeroCapacitatedBSP::getStartStationCapacitated(vector<StationidAndDemand> &mincostpath){
	vector<StationidAndDemand>::const_iterator it = mincostpath.begin();
	vector<StationidAndDemand>::const_iterator itt = mincostpath.begin();
	_startPoint = 0;
	if (checkSum()){
		//if (true){
		while (true){
			//cout << "*";
			// find a station which demond is positive :
			if ((*it).stationDemand >= 0){
				_startStationCapacitatedBSP = (*it).stationId;
				int tempSum = (*it).stationDemand;
				int tempNum = 1;

				while (tempSum >= 0 && tempNum < mincostpath.size()){
					++it;
					++tempNum;
					if (it == mincostpath.end()){
						it = mincostpath.begin();
					}
					tempSum += (*it).stationDemand;

				}

				if (tempNum == mincostpath.size()){
					//cout << endl;
					return _startStationCapacitatedBSP;
				}
			}

			if (++itt == mincostpath.end()){
				itt = mincostpath.begin();
			}
			it = itt;
			_startPoint++;
		}// while
	}// if

	return -1;
}

void NoZeroCapacitatedBSP::revertPath(vector<StationidAndDemand> &mincostpath){
	for (int i = 0; i < _startPoint; i++){
		StationidAndDemand temp;
		temp.isCutPoint = (*mincostpath.begin()).isCutPoint;
		temp.stationId = (*mincostpath.begin()).stationId;
		temp.stationDemand = (*mincostpath.begin()).stationDemand;
		mincostpath.erase(mincostpath.begin());
		mincostpath.push_back(temp);
	}
}

void NoZeroCapacitatedBSP::tryToMeetPositive(vector<StationidAndDemand> &mincostpath){
	vector<StationidAndDemand> temppath;
	int vehiclecapacity = 0;

	// set zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		vehiclecapacity += (*it).stationDemand;
		if ((*it).isCutPoint == PIECE_P){
			for (vector<StationidAndDemand>::iterator itt = it + 1; itt < mincostpath.end(); ++itt){
				if ((*itt).stationId == (*it).stationId && (*it).stationDemand != 0){
					if (vehiclecapacity + (*itt).stationDemand <= _tspBase.VEHICLE_CAPACITY){
						(*it).stationDemand += (*itt).stationDemand;
						(*itt).stationDemand = 0;
						vehiclecapacity += (*itt).stationDemand;
					}
					else {
						(*it).stationDemand += _tspBase.VEHICLE_CAPACITY - vehiclecapacity;
						(*itt).stationDemand -= _tspBase.VEHICLE_CAPACITY - vehiclecapacity;
						vehiclecapacity = _tspBase.VEHICLE_CAPACITY;
						break;
					}
				}
			}
		}
	}

	// delete zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		if ((*it).stationDemand != 0){
			StationidAndDemand temp;
			temp.isCutPoint = (*it).isCutPoint;
			temp.stationId = (*it).stationId;
			temp.stationDemand = (*it).stationDemand;
			temppath.push_back(temp);
		}
	}

	mincostpath.clear();
	for (vector<StationidAndDemand>::iterator it = temppath.begin(); it < temppath.end(); ++it){
		StationidAndDemand temp;
		temp.isCutPoint = (*it).isCutPoint;
		temp.stationId = (*it).stationId;
		temp.stationDemand = (*it).stationDemand;
		mincostpath.push_back(temp);
	}
}

void NoZeroCapacitatedBSP::tryToMeetNegative(vector<StationidAndDemand> &mincostpath){
	vector<StationidAndDemand> temppath;
	int vehiclecapacity = 0;

	// set zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		vehiclecapacity += (*it).stationDemand;
		if ((*it).isCutPoint == PIECE_N){
			for (vector<StationidAndDemand>::iterator itt = it + 1; itt < mincostpath.end(); ++itt){
				if ((*itt).stationId == (*it).stationId){
					if (vehiclecapacity + (*itt).stationDemand >= 0){
						//cout << "get one" << endl;
						(*it).stationDemand += (*itt).stationDemand;
						(*itt).stationDemand = 0;
						vehiclecapacity += (*itt).stationDemand;
					}
					else {
						//cout << "get part one" << endl;
						(*it).stationDemand += 0 - vehiclecapacity;
						(*itt).stationDemand += vehiclecapacity;
						vehiclecapacity = 0;
						break;
					}
				}
			}
		}
	}

	// delete zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		if ((*it).stationDemand != 0){
			StationidAndDemand temp;
			temp.isCutPoint = (*it).isCutPoint;
			temp.stationId = (*it).stationId;
			temp.stationDemand = (*it).stationDemand;
			temppath.push_back(temp);
		}
	}

	mincostpath.clear();
	for (vector<StationidAndDemand>::iterator it = temppath.begin(); it < temppath.end(); ++it){
		StationidAndDemand temp;
		temp.isCutPoint = (*it).isCutPoint;
		temp.stationId = (*it).stationId;
		temp.stationDemand = (*it).stationDemand;
		mincostpath.push_back(temp);
	}

}

void NoZeroCapacitatedBSP::mapPath(){
	for (vector<StationidAndDemand>::iterator it = _minCostPath.begin(); it < _minCostPath.end(); it++){
		(*it).stationId = _tspBase._mapPartToAll[(*it).stationId];
	}
}

void NoZeroCapacitatedBSP::deleteRepeatStationPoint(vector<StationidAndDemand> &mincostpath){
	vector<StationidAndDemand>::iterator it = mincostpath.begin();
	vector<StationidAndDemand> temp;
	temp.push_back(*it);
	for (it++; it < mincostpath.end(); ++it){
		if ((*it).stationId == temp[temp.size() - 1].stationId){
			temp[temp.size() - 1].stationDemand += (*it).stationDemand;
		}
		else{
			temp.push_back(*it);
		}
	}

	mincostpath.clear();
	for (vector<StationidAndDemand>::iterator it = temp.begin(); it < temp.end(); ++it){
		StationidAndDemand temp;
		temp.isCutPoint = (*it).isCutPoint;
		temp.stationId = (*it).stationId;
		temp.stationDemand = (*it).stationDemand;
		mincostpath.push_back(temp);
	}
}



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
	getSuperNodePieces(i);
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
	getSuperNodePieces(i);
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

void NoZeroCapacitatedBSP::printTspPath(){
	cout << "TSP tour:" << _tspBase._path.size() << endl;
	for (int i = 0; i < _tspBase._path.size(); i++){
		cout << _tspBase._path[i] << "(" << _tspBase._stationDemand[_tspBase._path[i]] << ") ";
	}
	cout << endl;
}

void NoZeroCapacitatedBSP::printFinalPath(){
	cout << "_minCostPath Path:" << _minCostPath.size() << endl;
	for (int i = 0; i < _minCostPath.size(); i++){
		cout << _minCostPath[i].stationId << "(" << _minCostPath[i].stationDemand << ") ";
	}

	int sum = 0;
	for (int i = 0; i < _minCostPath.size(); i++){
		sum += _minCostPath[i].stationDemand;
	}
	if (sum != 0){
		cout << "_minCostPath sum != 0!!!!!!!" << endl;
	}

	cout << endl;
}

void NoZeroCapacitatedBSP::printSuperNodeInformation(){

	cout << "_superNodeNumber_PIECE_P = " << _superNodeNumber_PIECE_P << endl;
	cout << "_superNodeNumber_PIECE_N = " << _superNodeNumber_PIECE_N << endl;
	cout << "_superNodeNumber_PIECE_0 = " << _superNodeNumber_PIECE_0 << endl;

	cout << "Positive super node:" << endl;
	for (int i = 0; i < _superNodeNumber / 2; i++){
		cout << "startit = " << *_superNodeVector_PIECE_P[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_P[i].getEndIt();
		cout << "  mincostpoint = " << _superNodeVector_PIECE_P[i].getMatchingNumber();
		//cout << "  matching point :" << *_minCostAmongSuperNode[i][_superNodeVector_PIECE_P[i].getMatchingNumber()].firstNodeIt << endl;
		cout << "  matching point :" << _minCostAmongSuperNode[i][_superNodeVector_PIECE_P[i].getMatchingNumber()].first << endl;
	}
	cout << "Negative super node:" << endl;
	for (int i = 0; i < _superNodeNumber / 2; i++){
		cout << "startit = " << *_superNodeVector_PIECE_N[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_N[i].getEndIt();
		cout << "  mincostpoint = " << _superNodeVector_PIECE_N[i].getMatchingNumber();
		//cout << "  matching point :" << *_minCostAmongSuperNode[_superNodeVector_PIECE_N[i].getMatchingNumber()][i].secondNodeIt << endl;
		cout << "  matching point :" << _minCostAmongSuperNode[_superNodeVector_PIECE_N[i].getMatchingNumber()][i].second << endl;
	}
	cout << "0 super node:" << endl;
	for (int i = 0; i < _superNodeNumber_PIECE_0; i++){
		cout << "startit = " << *_superNodeVector_PIECE_0[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_0[i].getEndIt();
	}
	cout << endl;
}

void NoZeroCapacitatedBSP::printTempPath(vector<StationidAndDemand> temp){
	int sum = 0;
	cout << "size:" << temp.size() << endl;
	for (int i = 0; i < temp.size(); i++){
		cout << temp[i].stationId << "(" << temp[i].stationDemand << ") ";
		sum += temp[i].stationDemand;
	}
	cout << "sum: " << sum << endl;
	cout << endl;
}
