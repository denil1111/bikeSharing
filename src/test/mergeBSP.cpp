 #include"mergeBSP.h"



MergeBSP::MergeBSP(int num) :TspBase(num){

	// used for UncapacitatedBSP:
	_startStationUncapacitatedBSP = -1;

	// used for CapacitatedBSP:
	_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_minSum = MM;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
}

MergeBSP::MergeBSP(int num, int x, int y) :TspBase(num, x, y){
	// used for UncapacitatedBSP:
	_startStationUncapacitatedBSP = -1;

	// used for CapacitatedBSP:
	_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_minSum = MM;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
}

MergeBSP::~MergeBSP(){

}

// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void MergeBSP::getTspTour(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	TSP alg(*g, *cost);
	_tspSum = alg.run();

	std::vector<Node> vec;
	alg.tourNodes(std::back_inserter(vec));
	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		FullGraph::Node node = *it;
		_path.push_back((*g).index(node));
	}

	PRINTFTspPath
}

bool MergeBSP::isExistNotVisitedPositiveStation(){
	return true;
}

bool MergeBSP::isAPositiveStation(int number){
	return _stationDemand[number] >= 0;
}

int MergeBSP::getStartStation(){
	_positiveStationVisiteFlag.clear();

	vector<int>::const_iterator it = _path.begin();
	while (isExistNotVisitedPositiveStation()){

		// if a positive demond station have been visited:
		vector<int>::iterator result = find(_positiveStationVisiteFlag.begin(), _positiveStationVisiteFlag.end(), *it);
		if (result != _positiveStationVisiteFlag.end()){
			cout << "No solution!!!!!" << endl;
			break;
		}


		// find a station which demond is positive:

		if (isAPositiveStation(*it)){
			_positiveStationVisiteFlag.push_back(*it);
			_startStationUncapacitatedBSP = *it;
			int tempSum = _stationDemand[*it];
			int tempNum = 1;

			while (tempSum >= 0 && tempNum < _stationNum){
				++it;
				++tempNum;
				if (it == _path.end()){
					it = _path.begin();
				}
				tempSum += _stationDemand[*it];

			}

			if (tempNum == _stationNum){
				PRINTFReuslt
					return _startStationUncapacitatedBSP;
			}

 

			if (tempSum < 0){
				++it;
				++tempNum;
				if (it == _path.end()){
					it = _path.begin();
				}
				while (_stationDemand[*it] < 0){
					tempSum += _stationDemand[*it];
					++it;
					if (it == _path.end()){
						it = _path.begin();
					}
				}
			}
		}// if
		else {
			++it;
		}
	}// while

	return -1;
}

void MergeBSP::getSuperNodePieces(int number){
	int startstation;
	// used for opposite direction:
	int previousupernode = PIECE_0;
	int previouszeronumber = 0;
	// used for forward direction to record last one super node:
	int lastsupernodeflag = PIECE_NULL;

	vector<int>::iterator it = _path.begin();
	for (int i = 0; i < number; i++){
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
	startstation = *it;

	//cout << "Start station id:" << startstation << " "<<_stationDemand[*it] <<endl << endl;
	int surplusdemand = 0;

	int flag = true;
	int stopflag = 0;
	while (stopflag < _stationNum){
		flag = false;

		SuperNode newpiece(Q / 2);
		surplusdemand = newpiece.getASuperNode(_path, _stationDemand, startstation, it, surplusdemand, stopflag, _stationNum);

		if (newpiece.getPieceTypeFlag() == PIECE_P){
			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;

			// used for opposite direction:
			if (previouszeronumber != 0){
				newpiece.setNumberOfZeroPieceFront(previouszeronumber);
				previouszeronumber = 0;
			}

			_superNodeVector_PIECE_P.push_back(newpiece);
			lastsupernodeflag = PIECE_P;

			// used for record first non-zero super node.
			if (_startFromWhichPiece == PIECE_0){
				_startFromWhichPiece = PIECE_P;
				//cout << "startpoint demond" << *newpiece.getStartIt() << " " << _stationDemand[*newpiece.getStartIt()] << endl;
			}

		}
		else if (newpiece.getPieceTypeFlag() == PIECE_N){
			_superNodeNumber_PIECE_N++;
			++_superNodeNumber;

			// used for opposite direction:
			if (previouszeronumber != 0){
				newpiece.setNumberOfZeroPieceFront(previouszeronumber);
				previouszeronumber = 0;
			}

			_superNodeVector_PIECE_N.push_back(newpiece);
			lastsupernodeflag = PIECE_N;

			// used for record first non-zero super node.
			if (_startFromWhichPiece == PIECE_0){
				_startFromWhichPiece = PIECE_N;
				//cout << "startpoint demond" << *newpiece.getStartIt() << " " << _stationDemand[*newpiece.getStartIt()] << endl;
			}

		}
		else{
			_superNodeVector_PIECE_0.push_back(newpiece);
			_superNodeNumber_PIECE_0++;

			previouszeronumber++;

			// used for forward direction to record zero pieces to P or N super node:
			if (lastsupernodeflag == PIECE_NULL){
				++_zeroSuperNodeNumberInFront;
			}
			else if (lastsupernodeflag == PIECE_P){
				(_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1].getNumberOfZeroPieceBehind())++;
			}
			else if (lastsupernodeflag == PIECE_N){
				(_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1].getNumberOfZeroPieceBehind())++;
			}
		}
	}

	// used for opposite direction: 
	_zeroSuperNodeNumberInEnd = previouszeronumber;

	//cout << "_zeroSuperNodeNumberInFront:" << _zeroSuperNodeNumberInFront << " _zeroSuperNodeNumberInEnd:" << _zeroSuperNodeNumberInEnd << endl << endl;
}

void MergeBSP::initMinCostAmongSuperNode(){
	for (int i = 0; i < _superNodeNumber / 2; i++){
		vector<MinCostOfTwoSuperNode> m;
		for (int j = 0; j < _superNodeNumber / 2; j++){
			MinCostOfTwoSuperNode minCostOfTwoSuperNode;
			minCostOfTwoSuperNode.cost = M;
			minCostOfTwoSuperNode.firstNodeIt = _path.end();
			minCostOfTwoSuperNode.secondNodeIt = _path.end();
			m.push_back(minCostOfTwoSuperNode);
		}
		_minCostAmongSuperNode.push_back(m);
	}
}

void MergeBSP::initSuperNode(){
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

void MergeBSP::calculateMinCostOfTwoSuperNode(int positivesupernode, int negativesupernode){
	// firstit and secondit point to _path.
	vector<int>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode].getStartIt();
	vector<int>::iterator negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
	FullGraph::Node unode, vnode;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt = positiveit;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt = negativeit;

	for (int i = 0; i < _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode(); i++){
		unode = (*g)(*positiveit);
		vnode = (*g)(*negativeit);
		if (unode == vnode){
			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
		}
		else{
			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = (*cost)[(*g).edge(unode, vnode)];
		}

		negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
		for (int j = 0; j < _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode(); j++){
			// FullGraph::NodeIt u(*g); (*cost)[(*g).edge(v, u)]
			// use node id get node,
			if (*positiveit == *negativeit){
				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt = positiveit;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt = negativeit;
				continue;
			}
			vnode = (*g)(*negativeit);
			int currentcost;
			if ((currentcost = (*cost)[(*g).edge(unode, vnode)]) < _minCostAmongSuperNode[positivesupernode][negativesupernode].cost){
				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = currentcost;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt = positiveit;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt = negativeit;
			}
			if (++negativeit == _path.end()){
				negativeit = _path.begin();
			}
		}
		if (++positiveit == _path.end()){
			positiveit = _path.begin();
		}
	}
	return;
}

void MergeBSP::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			calculateMinCostOfTwoSuperNode(i, j);
		}
	}
}

string MergeBSP::getLGF(){
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

void MergeBSP::machingSuperNode(){
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

void MergeBSP::pushbackStationidAndDemand(vector<StationidAndDemand> &tempVector, StationidAndDemand temp){
	int num = tempVector.size();
	if (num == 0){
		tempVector.push_back(temp);
	}
	/*else if (tempVector[num - 1].stationId == temp.stationId){
	int number = tempVector[num - 1].stationDemand;
	number += temp.stationDemand;

	tempVector[num - 1].stationDemand = number;
	cout << "ºÏ²¢£º" << tempVector[num - 1].stationId << endl;
	haha.push_back(temp);
	}*/
	/*else if ((*(tempVector.end() - 1)).stationId == temp.stationId){
	(*(tempVector.end() - 1)).stationDemand += temp.stationDemand;
	}*/
	else{
		tempVector.push_back(temp);
	}
}

void MergeBSP::getPositivePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void MergeBSP::getPositivePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void MergeBSP::getNegativePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void MergeBSP::getNegativePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void MergeBSP::getZeroPathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void MergeBSP::getZeroPath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void MergeBSP::getZeroPathInBehind(vector<StationidAndDemand> &tempVector){
	int zeronum = _superNodeVector_PIECE_0.size();
	int i = zeronum - 1;
	for (int j = 0; j < _zeroSuperNodeNumberInEnd; j++, i--){
		getZeroPathReverse(i, tempVector);
	}
}

void MergeBSP::getZeroPathInFront(vector<StationidAndDemand> &tempVector){
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPath(i, tempVector);
	}
}

void MergeBSP::getPathBeginPositiveReverse(){
	vector<StationidAndDemand> tempVector;

	int currentnumberofzeropiece = _superNodeVector_PIECE_0.size() - 1;
	if (_zeroSuperNodeNumberInEnd != 0){
		getZeroPathInBehind(tempVector);
		currentnumberofzeropiece -= _zeroSuperNodeNumberInEnd;
	}

	if (_superNodeNumber >= 2){
		for (int positivesupernode = _superNodeNumber / 2 - 1; positivesupernode >= 0; positivesupernode--){
			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();
			vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1;

			for (itt; itt > _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin(); --itt){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt)){
					getNegativePathReverse(negativesupernode, tempVector);

					// zero super node behind negative super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}
				}

				int numberofzeropiece = 0;
				if (itt == _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin()){
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}
				}
			}//	
			if (itt == _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin()){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt)){
					getNegativePathReverse(negativesupernode, tempVector);

					// zero super node behind negative super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}

					pushbackStationidAndDemand(tempVector, *itt);
					tempVector[tempVector.size() - 1].stationDemand = 0;
				}

				int numberofzeropiece = 0;
				if (itt == _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin()){
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}
				}
			}
		}// for positive 
	}// if (_superNodeNumber >= 2)

	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minMediumCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minMediumCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}

void MergeBSP::getPathBeginPositive(){
	vector<StationidAndDemand> tempVector;

	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront(tempVector);
		currentnumberofzeropiece += _zeroSuperNodeNumberInFront;
	}
	if (_superNodeNumber >= 2){
		for (int positivesupernode = 0; positivesupernode < _superNodeNumber / 2; ++positivesupernode){
			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();

			vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin();
			for (itt; itt < _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end(); ++itt){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt)){
					getNegativePath(negativesupernode, tempVector);

					// zero super node behind negative super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, tempVector);
							++currentnumberofzeropiece;
						}
					}

				}

				int numberofzeropiece = 0;
				if (itt == _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1){
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, tempVector);
							++currentnumberofzeropiece;
						}
					}
				}
			}//				
		}// for positive 
	}// if (_superNodeNumber >= 2)

	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minMediumCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minMediumCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}

void MergeBSP::getPathBeginNegativeReverse(){
	vector<StationidAndDemand> tempVector;

	int currentnumberofzeropiece = _superNodeVector_PIECE_0.size() - 1;
	if (_zeroSuperNodeNumberInEnd != 0){
		getZeroPathInBehind(tempVector);
		currentnumberofzeropiece -= _zeroSuperNodeNumberInEnd;
	}
	if (_superNodeNumber >= 2){
		for (int negativesupernode = _superNodeNumber / 2 - 1; negativesupernode >= 0; negativesupernode--){
			int positivesupernode = _superNodeVector_PIECE_N[negativesupernode].getMatchingNumber();

			vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1;
			for (itt; itt > _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin(); --itt){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt)){
					getPositivePathReverse(positivesupernode, tempVector);

					// zero super node behind positive super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}

				}

				if (itt == _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin()){
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}
				}
			}//	
			if (itt == _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin()){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt)){
					getPositivePathReverse(positivesupernode, tempVector);

					// zero super node behind positive super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}

				}

				if (itt == _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin()){
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, tempVector);
							--currentnumberofzeropiece;
						}
					}
				}
			}//	
		}// for positive 
	}// if (_superNodeNumber >= 2)

	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minMediumCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minMediumCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}

void MergeBSP::getPathBeginNegative(){
	vector<StationidAndDemand> tempVector;

	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront(tempVector);
		currentnumberofzeropiece += _zeroSuperNodeNumberInFront;
	}

	if (_superNodeNumber >= 2){
		for (int negativesupernode = 0; negativesupernode < _superNodeNumber / 2; ++negativesupernode){
			int positivesupernode = _superNodeVector_PIECE_N[negativesupernode].getMatchingNumber();

			vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin();
			for (itt; itt < _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end(); ++itt){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt)){
					getPositivePath(positivesupernode, tempVector);

					// zero super node behind positive super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, tempVector);
							++currentnumberofzeropiece;
						}
					}

					pushbackStationidAndDemand(tempVector, *itt);
					tempVector[tempVector.size() - 1].stationDemand = 0;
				}

				if (itt == _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1){
					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, tempVector);
							++currentnumberofzeropiece;
						}
					}
				}
			}//				
		}// for positive 
	}// if (_superNodeNumber >= 2)

	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minMediumCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minMediumCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}

void MergeBSP::getPath(){
	if (_startFromWhichPiece == PIECE_P){
		cout << "_startFromWhichPiece:PIECE_P" << endl;
		getPathBeginPositive();
		getPathBeginNegativeReverse();
	}
	else{
		cout << "_startFromWhichPiece:PIECE_N" << endl;
		getPathBeginNegative();
		getPathBeginPositiveReverse();
	}
}

int MergeBSP::getFinalSum(vector<StationidAndDemand> temp){
	FullGraph::Node unode, vnode;
	int i = 0;
	int tempsum = 0;
	vector<StationidAndDemand>::iterator it = temp.begin();

	unode = (*g)((*it++).stationId);
	for (; it < temp.end() - 1; ++it){
		vnode = (*g)((*it).stationId);
		if (vnode != unode){
			tempsum += (*cost)[(*g).edge(unode, vnode)];
		}
		unode = vnode;
	}

	return tempsum;
}

bool  MergeBSP::checkSum(){
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

int MergeBSP::getStartStationCapacitatedBSP(){
	return _startStationCapacitatedBSP;
}

int MergeBSP::getStartStationCapacitated(){
	vector<StationidAndDemand>::const_iterator it = _minCostPath.begin();
	vector<StationidAndDemand>::const_iterator itt = _minCostPath.begin();
	_startPoint = 0;
	if (checkSum()){
		//if (true){
		while (true){
			cout << "*";
			// find a station which demond is positive :
			if ((*it).stationDemand >= 0){
				_startStationCapacitatedBSP = (*it).stationId;
				int tempSum = (*it).stationDemand;
				int tempNum = 1;

				while (tempSum >= 0 && tempNum < _minCostPath.size()){
					++it;
					++tempNum;
					if (it == _minCostPath.end()){
						it = _minCostPath.begin();
					}
					tempSum += (*it).stationDemand;

				}

				if (tempNum == _minCostPath.size()){
					cout << endl;
					return _startStationCapacitatedBSP;
				}
			}

			if (++itt == _minCostPath.end()){
				itt = _minCostPath.begin();
			}
			it = itt;
			_startPoint++;
		}// while
	}// if

	return -1;
}

void MergeBSP::revertPath(){
	for (int i = 0; i < _startPoint; i++){
		StationidAndDemand temp;
		temp.stationId = (*_minCostPath.begin()).stationId;
		temp.stationDemand = (*_minCostPath.begin()).stationDemand;
		_minCostPath.erase(_minCostPath.begin());
		_minCostPath.push_back(temp);
	}
}

void MergeBSP::mapPath(){
	for (vector<StationidAndDemand>::iterator it = _minCostPath.begin(); it < _minCostPath.end(); it++){
		(*it).stationId = _mapPartToAll[(*it).stationId];
	}
}

void MergeBSP::deleteRepeatStationPoint(){
	vector<StationidAndDemand>::iterator it = _minMediumCostPath.begin();
	_minCostPath.push_back(*it);
	for (it++; it < _minMediumCostPath.end(); ++it){
		if ((*it).stationId == _minCostPath[_minCostPath.size() - 1].stationId){
			_minCostPath[_minCostPath.size() - 1].stationDemand += (*it).stationDemand;
		}
		else{
			_minCostPath.push_back(*it);
		}
	}
}

void MergeBSP::runRandom(){

	randomData();

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	totaltime0 = finish - sum;

	// Uncapacitated BSP:
	cout << endl << "UncapacitatedBSP:" << endl;
	sum = clock();
	start = clock();
	getStartStation();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetStartStation:" << totaltime << "ms!" << endl;

	totaltime1 = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;
	cout << "\nUncapacitated BSP time:" << totaltime1 << "ms! Sum cost:" << _tspSum << endl << endl;




	cout << endl << "CapacitatedBSP:" << endl;

	//for (int i = 0; i < Q / 2; i++){
	int i = 0;
	initSuperNode();
	getSuperNodePieces(i);
	calculateMinCostAmongSuperNode();
	machingSuperNode();
	getPath();
	cout << "Get a path " << i << endl << endl;
	//}
	deleteRepeatStationPoint();
	getStartStationCapacitated();
	revertPath();

	PRINTFFinalPath

	cout << "Mininum sum cost:" << _minSum << endl;
	cout << "StartStation:" << _startStationCapacitatedBSP << endl;
	cout << "length:" << _minMediumCostPath.size() << endl;
	cout << "Path set size:" << _pathSet.size() << endl;

}

void MergeBSP::run(){

	data();

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	totaltime0 = finish - sum;

	// Uncapacitated BSP:
	cout << endl << "UncapacitatedBSP:" << endl;
	sum = clock();
	start = clock();
	getStartStation();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetStartStation:" << totaltime << "ms!" << endl;

	totaltime1 = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;
	cout << "\nUncapacitated BSP time:" << totaltime1 << "ms! Sum cost:" << _tspSum << endl << endl;




	cout << endl << "CapacitatedBSP:" << endl;

	//for (int i = 0; i < Q / 2; i++){
	int i = 0;
	initSuperNode();
	getSuperNodePieces(i);
	calculateMinCostAmongSuperNode();
	machingSuperNode();
	getPath();
	cout << "Get a path " << i << endl << endl;
	//}
	deleteRepeatStationPoint();
	getStartStationCapacitated();
	revertPath();
	mapPath();

	PRINTFFinalPath

	cout << "Mininum sum cost:" << _minSum << endl;
	cout << "StartStation:" << _startStationCapacitatedBSP << endl;
	cout << "length:" << _minMediumCostPath.size() << endl;
	cout << "Path set size:" << _pathSet.size() << endl;

}

void MergeBSP::printTspPath(){
	cout << "TSP tour:" << _path.size() << endl;
	for (int i = 0; i < _path.size(); i++){
		cout << _path[i] << "(" << _stationDemand[_path[i]] << ") ";
	}
	cout << endl;
}

void MergeBSP::printFinalPath(){

	cout << "Final Path:" << _minMediumCostPath.size() << endl;
	for (int i = 0; i < _minMediumCostPath.size(); i++){
		cout << _minMediumCostPath[i].stationId << "(" << _minMediumCostPath[i].stationDemand << ") ";
	}
	cout << endl;

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

void MergeBSP::printSuperNodeInformation(){

	cout << "_superNodeNumber_PIECE_P = " << _superNodeNumber_PIECE_P << endl;
	cout << "_superNodeNumber_PIECE_N = " << _superNodeNumber_PIECE_N << endl;
	cout << "_superNodeNumber_PIECE_0 = " << _superNodeNumber_PIECE_0 << endl;

	cout << "Positive super node:" << endl;
	for (int i = 0; i < _superNodeNumber / 2; i++){
		cout << "startit = " << *_superNodeVector_PIECE_P[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_P[i].getEndIt();
		cout << "  mincostpoint = " << _superNodeVector_PIECE_P[i].getMatchingNumber();
		cout << "  matching point :" << *_minCostAmongSuperNode[i][_superNodeVector_PIECE_P[i].getMatchingNumber()].firstNodeIt << endl;
	}
	cout << "Negative super node:" << endl;
	for (int i = 0; i < _superNodeNumber / 2; i++){
		cout << "startit = " << *_superNodeVector_PIECE_N[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_N[i].getEndIt();
		cout << "  mincostpoint = " << _superNodeVector_PIECE_N[i].getMatchingNumber();
		cout << "  matching point :" << *_minCostAmongSuperNode[_superNodeVector_PIECE_N[i].getMatchingNumber()][i].secondNodeIt << endl;
	}
	cout << "0 super node:" << endl;
	for (int i = 0; i < _superNodeNumber_PIECE_0; i++){
		cout << "startit = " << *_superNodeVector_PIECE_0[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_0[i].getEndIt();
	}
	cout << endl;
}

void MergeBSP::printTempPath(vector<StationidAndDemand> temp){
	int sum = 0;
	cout << "size:" << temp.size() << endl;
	for (int i = 0; i < temp.size(); i++){
		cout << temp[i].stationId << "(" << temp[i].stationDemand << ") ";
		sum += temp[i].stationDemand;
	}
	cout << "sum: " << sum << endl;
	cout << endl;
}
