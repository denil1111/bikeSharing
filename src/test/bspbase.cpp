#include "bspbase.h"

BspBase::BspBase(){

}

BspBase::BspBase(TspBase &tspbase){
	_tspBase = tspbase;
	_startFromWhichPiece = PIECE_0;
	_superNodeNumber = 0;
}

BspBase::~BspBase(){

}

//// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
//template <typename TSP>
//void BspBase::getTspTour(const std::string &alg_name) {
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

void BspBase::initSuperNode(){
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

void BspBase::initMinCostAmongSuperNode(){
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

void BspBase::getSuperNodePieces(int number){
	int startstation;
	// used for opposite direction:
	int previousupernode = PIECE_0;
	int previouszeronumber = 0;
	// used for forward direction to record last one super node:
	int lastsupernodeflag = PIECE_NULL;

	vector<int>::iterator it = _tspBase._path.begin();
	for (int i = 0; i < number; i++){
		if (++it == _tspBase._path.end()){
			it = _tspBase._path.begin();
		}
	}
	startstation = *it;

	//cout << "Start station id:" << startstation << " "<<_stationDemand[*it] <<endl << endl;
	int surplusdemand = 0;

	int flag = true;
	int stopflag = 0;
	while (stopflag < _tspBase._stationNum){
		flag = false;

		SuperNode newpiece(_tspBase.VEHICLE_CAPACITY / 2);
		surplusdemand = newpiece.getASuperNode(_tspBase._path, _tspBase._stationDemand, startstation, it, surplusdemand, stopflag, _tspBase._stationNum);

		if (newpiece.getPieceTypeFlag() == PIECE_P){
			// used for calculate min cost between positive node and negative node:
			newpiece._positiveEnd = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._positiveReverseEnd = newpiece._stationidAndDemand[0].stationId;
			newpiece._positiveNext = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._positiveReverseNext = newpiece._stationidAndDemand[0].stationId;


			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;

			// used for opposite direction:
			if (previouszeronumber != 0){
				newpiece.setNumberOfZeroPieceFront(previouszeronumber);

				// used for calculate min cost between positive node and negative node:
				newpiece._positiveReverseEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - previouszeronumber]._stationidAndDemand[0].stationId;
				newpiece._positiveReverseNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;

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

			// used for calculate min cost between positive node and negative node:
			newpiece._negativeEnd = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._negativeReverseEnd = newpiece._stationidAndDemand[0].stationId;
			newpiece._negativeNext = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._negativeReverseNext = newpiece._stationidAndDemand[0].stationId;

			// used for opposite direction:
			if (previouszeronumber != 0){

				// used for calculate min cost between positive node and negative node:
				newpiece._negativeReverseEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - previouszeronumber]._stationidAndDemand[0].stationId;
				newpiece._negativeReverseNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;

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

				// used for calculate min cost between positive node and negative node:
				newpiece._positiveEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;
				newpiece._positiveNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - (_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1].getNumberOfZeroPieceBehind())]._stationidAndDemand[0].stationId;

			}
			else if (lastsupernodeflag == PIECE_N){
				(_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1].getNumberOfZeroPieceBehind())++;

				// used for calculate min cost between positive node and negative node:
				newpiece._negativeEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;
				newpiece._negativeNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - (_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1].getNumberOfZeroPieceBehind())]._stationidAndDemand[0].stationId;
			}
		}
	}

	// used for opposite direction: 
	_zeroSuperNodeNumberInEnd = previouszeronumber;

	//cout << "_zeroSuperNodeNumberInFront:" << _zeroSuperNodeNumberInFront << " _zeroSuperNodeNumberInEnd:" << _zeroSuperNodeNumberInEnd << endl << endl;
}

void BspBase::getSuperNodePiecesNoZero(int number){
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

void BspBase::calculateMinCostOfTwoSuperNode(int positivesupernode, int negativesupernode){
	// firstit and secondit point to _tspBase._path.
	vector<int>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode].getStartIt();
	vector<int>::iterator negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
	FullGraph::Node unode, vnode;

	_minCostAmongSuperNode[positivesupernode][negativesupernode].first = *positiveit;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].second = *negativeit;

	for (int i = 0; i < _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode(); i++){
		unode = (*_tspBase.g)(*positiveit);
		vnode = (*_tspBase.g)(*negativeit);
		if (unode == vnode){
			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
		}
		else{
			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)];
		}

		negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
		for (int j = 0; j < _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode(); j++){
			// FullGraph::NodeIt u(*_tspBase.g); (*_tspBase.cost)[(*_tspBase.g).edge(v, u)]
			// use node id get node,
			if (*positiveit == *negativeit){
				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].first = *positiveit;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].second = *negativeit;
				continue;
			}
			vnode = (*_tspBase.g)(*negativeit);
			int currentcost;
			if ((currentcost = (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)]) < _minCostAmongSuperNode[positivesupernode][negativesupernode].cost){
				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = currentcost;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].first = *positiveit;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].second = *negativeit;
			}
			if (++negativeit == _tspBase._path.end()){
				negativeit = _tspBase._path.begin();
			}
		}
		if (++positiveit == _tspBase._path.end()){
			positiveit = _tspBase._path.begin();
		}
	}
	return;
}

void BspBase::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			calculateMinCostOfTwoSuperNode(i, j);
		}
	}
}

string BspBase::getLGF(){
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

void BspBase::machingSuperNode(){
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

void BspBase::pushbackStationidAndDemand(vector<StationidAndDemand> &tempVector, StationidAndDemand temp){
	int num = tempVector.size();
	if (num == 0){
		tempVector.push_back(temp);
	}
	else{
		tempVector.push_back(temp);
	}
}

void BspBase::getPositivePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void BspBase::getNegativePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void BspBase::getZeroPath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void BspBase::getPositivePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void BspBase::getNegativePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void BspBase::getZeroPathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	// itt == _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	pushbackStationidAndDemand(tempVector, *itt);
}

void BspBase::getZeroPathInBehind(vector<StationidAndDemand> &tempVector){
	int zeronum = _superNodeVector_PIECE_0.size();
	int i = zeronum - 1;
	for (int j = 0; j < _zeroSuperNodeNumberInEnd; j++, i--){
		getZeroPathReverse(i, tempVector);
	}
}

void BspBase::getZeroPathInFront(vector<StationidAndDemand> &tempVector){
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPath(i, tempVector);
	}
}

void BspBase::getPathBeginPositiveReverse(){
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
				//if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt)){
				if ((*itt).stationId == _minCostAmongSuperNode[positivesupernode][negativesupernode].first){
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
				//if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt)){
				if ((*itt).stationId == _minCostAmongSuperNode[positivesupernode][negativesupernode].first){
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

	deleteRepeatStationPoint(tempVector);
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);
	//tryToMeetNegative(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.isCutPoint = (*it).isCutPoint;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}

void BspBase::getPathBeginPositive(){
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
				//if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt)){
				if ((*itt).stationId == _minCostAmongSuperNode[positivesupernode][negativesupernode].first){
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
					// get zero path behind the positive piece:
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

	deleteRepeatStationPoint(tempVector);
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);
	//tryToMeetPositive(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.isCutPoint = (*it).isCutPoint;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}

void BspBase::getPathBeginNegativeReverse(){
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
				//if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt)){
				if ((*itt).stationId == _minCostAmongSuperNode[positivesupernode][negativesupernode].second){
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
				//if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt)){
				if ((*itt).stationId == _minCostAmongSuperNode[positivesupernode][negativesupernode].second){
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

	deleteRepeatStationPoint(tempVector);
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);
	//tryToMeetNegative(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.isCutPoint = (*it).isCutPoint;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}

void BspBase::getPathBeginNegative(){
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
				//if ((*itt).stationId == *(_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt)){
				if ((*itt).stationId == _minCostAmongSuperNode[positivesupernode][negativesupernode].second){
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

	deleteRepeatStationPoint(tempVector);
	getStartStationCapacitated(tempVector);
	revertPath(tempVector);
	//tryToMeetPositive(tempVector);
	_pathSet.push_back(tempVector);

	int tempsum = 0;
	if ((tempsum = getFinalSum(tempVector)) < _minSum){

		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<StationidAndDemand>::iterator it = tempVector.begin(); it < tempVector.end(); ++it){
			StationidAndDemand tt;
			tt.isCutPoint = (*it).isCutPoint;
			tt.stationId = (*it).stationId;
			tt.stationDemand = (*it).stationDemand;
			_minCostPath.push_back(tt);
		}
	}

	PRINTFTempPath

}




int BspBase::getStartStationCapacitated(vector<StationidAndDemand> &mincostpath){
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

bool  BspBase::checkSum(){
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

void BspBase::deleteRepeatStationPoint(vector<StationidAndDemand> &mincostpath){
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

void BspBase::revertPath(vector<StationidAndDemand> &mincostpath){
	for (int i = 0; i < _startPoint; i++){
		StationidAndDemand temp;
		temp.isCutPoint = (*mincostpath.begin()).isCutPoint;
		temp.stationId = (*mincostpath.begin()).stationId;
		temp.stationDemand = (*mincostpath.begin()).stationDemand;
		mincostpath.erase(mincostpath.begin());
		mincostpath.push_back(temp);
	}
}

void BspBase::tryToMeetPositive(vector<StationidAndDemand> &mincostpath){
	vector<StationidAndDemand> temppath;
	int vehiclecapacity = 0;

	// set zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		vehiclecapacity += (*it).stationDemand;
		if ((*it).isCutPoint == PIECE_P){
			for (vector<StationidAndDemand>::iterator itt = it + 1; itt < mincostpath.end(); ++itt){
				if ((*itt).stationId == (*it).stationId){
					if (vehiclecapacity + (*itt).stationDemand <= _tspBase.VEHICLE_CAPACITY){
						//	cout << (*itt).stationId << "ha(" << (*itt).stationDemand;
						vehiclecapacity += (*itt).stationDemand;
						//	cout << " vv" << vehiclecapacity << " ";
						(*it).stationDemand += (*itt).stationDemand;
						(*itt).stationDemand = 0;
						//	cout << "," << (*itt).stationDemand << ") ";
					}
					else {
						//cout << (*itt).stationId << "(" << (*itt).stationDemand;
						//cout << " vv"<<vehiclecapacity<<" ";
						(*it).stationDemand += _tspBase.VEHICLE_CAPACITY - vehiclecapacity;
						(*itt).stationDemand -= _tspBase.VEHICLE_CAPACITY - vehiclecapacity;
						vehiclecapacity = _tspBase.VEHICLE_CAPACITY;
						//cout << "," << (*itt).stationDemand << ") ";
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

	int sum = 0;
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		sum += (*it).stationDemand;
		if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
			cout << "tryToMeetPositive cao!!!!!!!!!!!!" << endl;
			break;
		}
	}
}

void BspBase::tryToMeetNegative(vector<StationidAndDemand> &mincostpath){
	vector<StationidAndDemand> temppath;
	int vehiclecapacity = 0;

	// set zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		vehiclecapacity += (*it).stationDemand;
		//cout << " lala" << vehiclecapacity << " ";

		if ((*it).isCutPoint == PIECE_N && (*it).stationDemand != 0){
			for (vector<StationidAndDemand>::iterator itt = it + 1; itt < mincostpath.end(); ++itt){
				if ((*itt).stationId == (*it).stationId){
					if (vehiclecapacity + (*itt).stationDemand >= 0){
						//cout << (*itt).stationId << "ha(" << (*itt).stationDemand;
						vehiclecapacity += (*itt).stationDemand;
						//cout << " tt" << vehiclecapacity << " ";
						(*it).stationDemand += (*itt).stationDemand;
						(*itt).stationDemand = 0;
						//cout << "," << (*itt).stationDemand << ") ";
					}
					else {
						//cout << (*itt).stationId << "(" << (*itt).stationDemand;
						//cout << " vv" << vehiclecapacity << " ";
						(*it).stationDemand += 0 - vehiclecapacity;
						(*itt).stationDemand += vehiclecapacity;
						vehiclecapacity = 0;
						//	cout << "," << (*itt).stationDemand << ") ";
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

	int sum = 0;
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		sum += (*it).stationDemand;
		if (sum < 0 || sum > _tspBase.VEHICLE_CAPACITY){
			cout << "tryToMeetNegative cao!!!!!!!!!!!!" << endl;
			break;
		}
	}

}

void BspBase::mapPath(){
	for (vector<StationidAndDemand>::iterator it = _minCostPath.begin(); it < _minCostPath.end(); it++){
		(*it).stationId = _tspBase._mapPartToAll[(*it).stationId];
	}
}

void BspBase::getPath(){
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

int BspBase::getFinalSum(vector<StationidAndDemand> temp){
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



void BspBase::printTspPath(){
	cout << "TSP tour:" << _tspBase._path.size() << endl;
	for (int i = 0; i < _tspBase._path.size(); i++){
		cout << _tspBase._path[i] << "(" << _tspBase._stationDemand[_tspBase._path[i]] << ") ";
	}
	cout << endl;
}

void BspBase::printFinalPath(){

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

void BspBase::printSuperNodeInformation(){

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

void BspBase::printTempPath(vector<StationidAndDemand> temp){
	int sum = 0;
	cout << "size:" << temp.size() << endl;
	for (int i = 0; i < temp.size(); i++){
		cout << temp[i].stationId << "(" << temp[i].stationDemand << ") ";
		sum += temp[i].stationDemand;
	}
	cout << "sum: " << sum << endl;
	cout << endl;
}
