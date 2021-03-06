#include "bspbase.h"

BspBase::BspBase(){

}

BspBase::BspBase(TspBase &tspbase){
	_tspBase = tspbase;
	_startFromWhichPiece = PIECE_0;
	_superNodeNumber = 0;
	_minSum = MM;
}

BspBase::~BspBase(){

}

void BspBase::initSuperNode(){
	_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
	_zeroSuperNodeNumberInEnd = 0;
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
			m.push_back(minCostOfTwoSuperNode);
		}
		_minCostAmongSuperNode.push_back(m);
	}
}

void BspBase::getSuperNodePieces(int number){
	initSuperNode();
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
		surplusdemand = newpiece.getASuperNode(_tspBase._path, it, _tspBase._stationDemand, surplusdemand, startstation, stopflag, _tspBase._stationNum);
		if (newpiece._pieceTypeFlag == PIECE_P){
			// used for calculate min cost between positive node and negative node:
			newpiece._positiveEnd = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._positiveReverseEnd = newpiece._stationidAndDemand[0].stationId;
			newpiece._positiveNext = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._positiveReverseNext = newpiece._stationidAndDemand[0].stationId;


			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;

			// used for opposite direction:
			if (previouszeronumber != 0){
				newpiece._numberOfZeroPieceFront = previouszeronumber;
				for (int i = 0; i < previouszeronumber; i++){
					newpiece._zeroPieceIndexFront.push_back(_superNodeVector_PIECE_0.size() - 1 - i);
				}

				// used for calculate min cost between positive node and negative node:
				newpiece._positiveReverseEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - previouszeronumber]._stationidAndDemand[0].stationId;
				newpiece._positiveReverseNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;

				previouszeronumber = 0;
			}

			for (int j = 0; j < newpiece._stationidAndDemand.size(); j++){
				newpiece._stationidAndDemand[j].pieceType = PIECE_P;
				newpiece._stationidAndDemand[j].pieceId = _superNodeNumber_PIECE_P - 1;
			}
			_superNodeVector_PIECE_P.push_back(newpiece);
			lastsupernodeflag = PIECE_P;

			// used for record first non-zero super node.
			if (_startFromWhichPiece == PIECE_0){
				_startFromWhichPiece = PIECE_P;
			}

		}
		else if (newpiece._pieceTypeFlag == PIECE_N){
			// used for calculate min cost between positive node and negative node:
			newpiece._negativeEnd = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._negativeReverseEnd = newpiece._stationidAndDemand[0].stationId;
			newpiece._negativeNext = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._negativeReverseNext = newpiece._stationidAndDemand[0].stationId;


			_superNodeNumber_PIECE_N++;
			++_superNodeNumber;

			// used for opposite direction:
			if (previouszeronumber != 0){
				newpiece._numberOfZeroPieceFront = previouszeronumber;
				for (int i = 0; i < previouszeronumber; i++){
					newpiece._zeroPieceIndexFront.push_back(_superNodeVector_PIECE_0.size() - 1 - i);
				}

				// used for calculate min cost between positive node and negative node:
				newpiece._negativeReverseEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - previouszeronumber]._stationidAndDemand[0].stationId;
				newpiece._negativeReverseNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;

				previouszeronumber = 0;
			}

			for (int j = 0; j < newpiece._stationidAndDemand.size(); j++){
				newpiece._stationidAndDemand[j].pieceType = PIECE_N;
				newpiece._stationidAndDemand[j].pieceId = _superNodeNumber_PIECE_N - 1;
			}
			_superNodeVector_PIECE_N.push_back(newpiece);
			lastsupernodeflag = PIECE_N;

			// used for record first non-zero super node.
			if (_startFromWhichPiece == PIECE_0){
				_startFromWhichPiece = PIECE_N;
				// cout << "startpoint demond" << *newpiece.getStartIt() << " " << _tspBase._stationDemand[*newpiece.getStartIt()] << endl;
			}

		}
		else{//PIECE_0
			_superNodeNumber_PIECE_0++;
			for (int j = 0; j < newpiece._stationidAndDemand.size(); j++){
				newpiece._stationidAndDemand[j].pieceType = PIECE_0;
				newpiece._stationidAndDemand[j].pieceId = _superNodeNumber_PIECE_0 - 1;
			}
			_superNodeVector_PIECE_0.push_back(newpiece);
			previouszeronumber++;

			// used for forward direction to record zero pieces to P or N super node:
			if (lastsupernodeflag == PIECE_NULL){
				++_zeroSuperNodeNumberInFront;
			}
			else if (lastsupernodeflag == PIECE_P){
				(_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._numberOfZeroPieceBehind)++;
				_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._zeroPieceIndexBehind.push_back(_superNodeVector_PIECE_0.size() - 1);

				// used for calculate min cost between positive node and negative node:
				_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._positiveEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;
				_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._positiveNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - (_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._numberOfZeroPieceBehind)]._stationidAndDemand[0].stationId;

			}
			else if (lastsupernodeflag == PIECE_N){
				(_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._numberOfZeroPieceBehind)++;
				_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._zeroPieceIndexBehind.push_back(_superNodeVector_PIECE_0.size() - 1);

				// used for calculate min cost between positive node and negative node:
				_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._negativeEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;
				_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._negativeNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - (_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._numberOfZeroPieceBehind)]._stationidAndDemand[0].stationId;
			}

		}
	}

	// used for opposite direction:
	_zeroSuperNodeNumberInEnd = previouszeronumber;

	//cout << "_zeroSuperNodeNumberInFront:" << _zeroSuperNodeNumberInFront << " _zeroSuperNodeNumberInEnd:" << _zeroSuperNodeNumberInEnd << endl << endl;
}

void BspBase::getSuperNodePiecesZeroOnlyAfterPositive(int number){
	initSuperNode();
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
		surplusdemand = newpiece.getASuperNode(_tspBase._path, it, _tspBase._stationDemand, surplusdemand, startstation, stopflag, _tspBase._stationNum);
		// cout<<"stopflag:"<<stopflag<<endl;
		if (newpiece._pieceTypeFlag == PIECE_P){
			// used for calculate min cost between positive node and negative node:
			newpiece._positiveEnd = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._positiveReverseEnd = newpiece._stationidAndDemand[0].stationId;
			newpiece._positiveNext = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._positiveReverseNext = newpiece._stationidAndDemand[0].stationId;


			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;

			// used for opposite direction:
			if (previouszeronumber != 0 && _startFromWhichPiece == PIECE_N){
				newpiece._numberOfZeroPieceFront = previouszeronumber;
				for (int i = 0; i < previouszeronumber; i++){
					newpiece._zeroPieceIndexFront.push_back(_superNodeVector_PIECE_0.size() - 1 - i);
				}

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
				// cout << "startpoint demond" << *newpiece.getStartIt() << " " << _tspBase._stationDemand[*newpiece.getStartIt()] << endl;
			}

		}
		else if (newpiece._pieceTypeFlag == PIECE_N){
			// used for calculate min cost between positive node and negative node:
			newpiece._negativeEnd = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._negativeReverseEnd = newpiece._stationidAndDemand[0].stationId;
			newpiece._negativeNext = newpiece._stationidAndDemand[newpiece._stationidAndDemand.size() - 1].stationId;
			newpiece._negativeReverseNext = newpiece._stationidAndDemand[0].stationId;


			_superNodeNumber_PIECE_N++;
			++_superNodeNumber;

			// used for opposite direction:
			if (previouszeronumber != 0 && _startFromWhichPiece == PIECE_P){
				newpiece._numberOfZeroPieceFront = previouszeronumber;
				for (int i = 0; i < previouszeronumber; i++){
					newpiece._zeroPieceIndexFront.push_back(_superNodeVector_PIECE_0.size() - 1 - i);
				}

				// used for calculate min cost between positive node and negative node:
				newpiece._negativeReverseEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - previouszeronumber]._stationidAndDemand[0].stationId;
				newpiece._negativeReverseNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;

				previouszeronumber = 0;
			}

			_superNodeVector_PIECE_N.push_back(newpiece);
			lastsupernodeflag = PIECE_N;

			// used for record first non-zero super node.
			if (_startFromWhichPiece == PIECE_0){
				_startFromWhichPiece = PIECE_N;
				// cout << "startpoint demond" << *newpiece.getStartIt() << " " << _tspBase._stationDemand[*newpiece.getStartIt()] << endl;
			}

		}
		else{//PIECE_0
			_superNodeNumber_PIECE_0++;
			_superNodeVector_PIECE_0.push_back(newpiece);
			previouszeronumber++;

			// used for forward direction to record zero pieces to P or N super node:
			if (lastsupernodeflag == PIECE_NULL){
				++_zeroSuperNodeNumberInFront;
			}
			else if (_startFromWhichPiece == PIECE_P){
				(_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._numberOfZeroPieceBehind)++;
				_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._zeroPieceIndexBehind.push_back(_superNodeVector_PIECE_0.size() - 1);

				// used for calculate min cost between positive node and negative node:
				_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._positiveEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;
				_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._positiveNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - (_superNodeVector_PIECE_P[_superNodeVector_PIECE_P.size() - 1]._numberOfZeroPieceBehind)]._stationidAndDemand[0].stationId;

			}
			else if (_startFromWhichPiece == PIECE_N){
				(_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._numberOfZeroPieceBehind)++;
				_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._zeroPieceIndexBehind.push_back(_superNodeVector_PIECE_0.size() - 1);

				// used for calculate min cost between positive node and negative node:
				_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._negativeEnd = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand[_superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - 1]._stationidAndDemand.size() - 1].stationId;
				_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._negativeNext = _superNodeVector_PIECE_0[_superNodeVector_PIECE_0.size() - (_superNodeVector_PIECE_N[_superNodeVector_PIECE_N.size() - 1]._numberOfZeroPieceBehind)]._stationidAndDemand[0].stationId;
			}

		}
	}

	// used for opposite direction:
	_zeroSuperNodeNumberInEnd = previouszeronumber;

	//cout << "_zeroSuperNodeNumberInFront:" << _zeroSuperNodeNumberInFront << " _zeroSuperNodeNumberInEnd:" << _zeroSuperNodeNumberInEnd << endl << endl;
}

void BspBase::getSuperNodePiecesNoZero(int number){
	initSuperNode();
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
		surplusdemand = newpiece.getASuperNodeNoZero(_tspBase._path, it, _tspBase._stationDemand, surplusdemand, startstation, stopflag, _tspBase._stationNum);

		if (newpiece._pieceTypeFlag == PIECE_P){
			if (flag == true){
				flag = false;
				_startFromWhichPiece = PIECE_P;
			}

			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;

			_superNodeVector_PIECE_P.push_back(newpiece);
			lastsupernodeflag = PIECE_P;

		}
		else if (newpiece._pieceTypeFlag == PIECE_N){
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
	vector<StationidAndDemand>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin();
	vector<StationidAndDemand>::iterator negativeit = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin();
	FullGraph::Node unode, vnode;

	_minCostAmongSuperNode[positivesupernode][negativesupernode].first = positiveit->stationId;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].second = negativeit->stationId;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = M-1;
	bool breakFlag = false;

	int positivestationnumber = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.size();
	for (int i = 0; i < positivestationnumber; i++, positiveit++){
		if (breakFlag) break;
		unode = (*_tspBase.g)(positiveit->stationId);
		// vnode = (*_tspBase.g)(*negativeit);
		negativeit = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin();
		int negativestationnumber = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.size();
		for (int j = 0; j < negativestationnumber; j++, negativeit++){
			// FullGraph::NodeIt u(*_tspBase.g); (*_tspBase.cost)[(*_tspBase.g).edge(v, u)]
			// use node id get node,
			if (positiveit->stationId == negativeit->stationId){
				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].first = positiveit->stationId;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].second = negativeit->stationId;
				breakFlag = true;
				break;
			}
			vnode = (*_tspBase.g)(negativeit->stationId);
			int currentcost = (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)];

			if ( currentcost < _minCostAmongSuperNode[positivesupernode][negativesupernode].cost){
				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = currentcost;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].first = positiveit->stationId;
				_minCostAmongSuperNode[positivesupernode][negativesupernode].second = negativeit->stationId;
			}
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
				_superNodeVector_PIECE_N[number_N - _superNodeNumber / 2]._matchingNumber = graph.id(mwpm.mate(n));
				_superNodeVector_PIECE_P[number_P]._matchingNumber = number_N - _superNodeNumber / 2;
			}
		}
	}

	PRINTFSuperNode

}

void BspBase::pushbackStationidAndDemand(vector<StationidAndDemand> &tempVector, StationidAndDemand temp){
	tempVector.push_back(temp);
}

void BspBase::getZeroPath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
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


void BspBase::getSuperPath(SuperNode super,vector<StationidAndDemand> &tempVector)
{
	for (int i=0;i<super._stationidAndDemand.size();i++)
	{
		pushbackStationidAndDemand(tempVector, super._stationidAndDemand[i]);
	}
}

void BspBase::getSuperPathReverse(SuperNode super,vector<StationidAndDemand> &tempVector)
{
	for (int i=0;i<super._stationidAndDemand.size();i++)
	{
		pushbackStationidAndDemand(tempVector, super._stationidAndDemand[super._stationidAndDemand.size() - 1 - i]);
	}
}



void BspBase::getOrderPath(vector<SuperNode> firstPIECE,vector<SuperNode> secondPIECE){
	vector<StationidAndDemand> tempVector;

	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront(tempVector);
		currentnumberofzeropiece += _zeroSuperNodeNumberInFront;
	}

	if (_superNodeNumber >= 2){
		for (int first = 0; first < _superNodeNumber / 2; ++first){
			int second = firstPIECE[first]._matchingNumber;

			vector<StationidAndDemand>::iterator itt = firstPIECE[first]._stationidAndDemand.begin();
			for (; itt < firstPIECE[first]._stationidAndDemand.end(); ++itt){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				int matching;
				if (_startFromWhichPiece == PIECE_P){
					matching = _minCostAmongSuperNode[first][second].first;
				}
				else{
					matching = _minCostAmongSuperNode[second][first].second;
				}
				//matching = _minCostAmongSuperNode[first][second].first;
				if ((*itt).stationId == matching){
					getSuperPath(secondPIECE[second], tempVector);

					// zero super node behind negative super node:
					int numberofzeropiece = secondPIECE[second]._numberOfZeroPieceBehind;
					if (numberofzeropiece!= 0){
						cout << "getOrderPath() wrong!!!!! in bspbase.cpp" << endl;
						for (int i = 0; i < secondPIECE[second]._zeroPieceIndexBehind.size(); i++){
							int number = secondPIECE[second]._zeroPieceIndexBehind[i];
							if (_startFromWhichPiece == PIECE_P){
								if(i == secondPIECE[second]._zeroPieceIndexBehind.size()-1 && _superNodeVector_PIECE_0[number]._stationidAndDemand[0].stationDemand >= 0){
									getZeroPath(secondPIECE[second]._zeroPieceIndexBehind[i], tempVector);
								}else{
									getZeroPathReverse(secondPIECE[second]._zeroPieceIndexBehind[i], tempVector);
								}
							}else{
								if(i == secondPIECE[second]._zeroPieceIndexBehind.size()-1 && _superNodeVector_PIECE_0[number]._stationidAndDemand[0].stationDemand < 0){
									getZeroPath(secondPIECE[second]._zeroPieceIndexBehind[i], tempVector);
								}else{
									getZeroPathReverse(secondPIECE[second]._zeroPieceIndexBehind[i], tempVector);
								}
							}
							++currentnumberofzeropiece;
						}
					}
				}

			}
			// get zero path behind the positive piece:
			int numberofzeropiece = firstPIECE[first]._numberOfZeroPieceBehind;
			if (numberofzeropiece!= 0){
				for (int i = 0; i < firstPIECE[first]._zeroPieceIndexBehind.size(); i++){
					getZeroPath(firstPIECE[first]._zeroPieceIndexBehind[i], tempVector);
					++currentnumberofzeropiece;
				}
			}
		}
	}

	getStartStation(tempVector,_minCostPath,_minSum);

	// PRINTFTempPath

}

void BspBase::getReversePath(vector<SuperNode> firstPIECE, vector<SuperNode> secondPIECE){
	vector<StationidAndDemand> tempVector;

	int currentnumberofzeropiece = _superNodeVector_PIECE_0.size() - 1;
	if (_zeroSuperNodeNumberInEnd != 0){
		getZeroPathInBehind(tempVector);
		currentnumberofzeropiece -= _zeroSuperNodeNumberInEnd;
	}

	if (_superNodeNumber >= 2){
		for (int first = _superNodeNumber / 2 - 1; first >= 0; first--){
			int second = firstPIECE[first]._matchingNumber;
			vector<StationidAndDemand>::iterator itt = firstPIECE[first]._stationidAndDemand.end() - 1;

			for ( ; itt > firstPIECE[first]._stationidAndDemand.begin(); --itt){
				pushbackStationidAndDemand(tempVector, *itt);

				int matching;
				if (_startFromWhichPiece == PIECE_P){
					matching = _minCostAmongSuperNode[second][first].second;
				}
				else{
					matching = _minCostAmongSuperNode[first][second].first;
				}
				if ((*itt).stationId == matching){
					//getNegativePathReverse(second, tempVector);
					getSuperPathReverse(secondPIECE[second], tempVector);

					// zero super node behind negative super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = secondPIECE[second]._numberOfZeroPieceFront) != 0){
						cout << "getReversePath() wrong!!!!! in bspbase.cpp" << endl;
						for (int i = 0; i < secondPIECE[second]._zeroPieceIndexFront.size(); i++){



							int number = secondPIECE[second]._zeroPieceIndexFront[i];
							if (_startFromWhichPiece == PIECE_P){
								if (i == secondPIECE[second]._zeroPieceIndexFront.size() - 1 && _superNodeVector_PIECE_0[number]._stationidAndDemand[0].stationDemand < 0){
									getZeroPath(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
								else{
									getZeroPathReverse(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
							}
							else{
								if (i == secondPIECE[second]._zeroPieceIndexFront.size() - 1 && _superNodeVector_PIECE_0[number]._stationidAndDemand[0].stationDemand >= 0){
									getZeroPath(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
								else{
									getZeroPathReverse(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
							}



							//getZeroPathReverse(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
							--currentnumberofzeropiece;
						}
					}
				}
			}
			if (itt == firstPIECE[first]._stationidAndDemand.begin()){
				pushbackStationidAndDemand(tempVector, *itt);
				// matching station:
				int matching;
				if (_startFromWhichPiece == PIECE_P){
					matching = _minCostAmongSuperNode[second][first].second;
				}
				else{
					matching = _minCostAmongSuperNode[first][second].first;
				}
				if ((*itt).stationId == matching){
					getSuperPathReverse(secondPIECE[second], tempVector);

					// zero super node behind negative super node:
					int numberofzeropiece = 0;
					if ((numberofzeropiece = secondPIECE[second]._numberOfZeroPieceFront) != 0){
						for (int i = 0; i < secondPIECE[second]._zeroPieceIndexFront.size(); i++){


							int number = secondPIECE[second]._zeroPieceIndexFront[i];
							if (_startFromWhichPiece == PIECE_P){
								if (i == secondPIECE[second]._zeroPieceIndexFront.size() - 1 && _superNodeVector_PIECE_0[number]._stationidAndDemand[0].stationDemand < 0){
									getZeroPath(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
								else{
									getZeroPathReverse(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
							}
							else{
								if (i == secondPIECE[second]._zeroPieceIndexFront.size() - 1 && _superNodeVector_PIECE_0[number]._stationidAndDemand[0].stationDemand >= 0){
									getZeroPath(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
								else{
									getZeroPathReverse(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
								}
							}


							//getZeroPathReverse(secondPIECE[second]._zeroPieceIndexFront[i], tempVector);
							--currentnumberofzeropiece;
						}
					}
				}
			}

			int numberofzeropiece = 0;
			if (itt == firstPIECE[first]._stationidAndDemand.begin()){
				if ((numberofzeropiece = firstPIECE[first]._numberOfZeroPieceFront) != 0){
					for (int i = 0; i < firstPIECE[first]._zeroPieceIndexFront.size(); i++){
						getZeroPathReverse(firstPIECE[first]._zeroPieceIndexFront[i], tempVector);
						--currentnumberofzeropiece;
					}
				}
			}
		}
	}

	getStartStation(tempVector, _minCostPath, _minSum);

}



void BspBase::getPath(){
	if (_startFromWhichPiece == PIECE_P){
		//cout << "_startFromWhichPiece:PIECE_P" << endl;
		getOrderPath(_superNodeVector_PIECE_P,_superNodeVector_PIECE_N);
		getReversePath(_superNodeVector_PIECE_N, _superNodeVector_PIECE_P);
	}
	else{
		//cout << "_startFromWhichPiece:PIECE_N" << endl;
		getOrderPath(_superNodeVector_PIECE_N,_superNodeVector_PIECE_P);
		getReversePath(_superNodeVector_PIECE_P, _superNodeVector_PIECE_N);
	}
}


int BspBase::getStartStation(vector<StationidAndDemand> &mincostpath,vector<StationidAndDemand> &resultpath,int &minSum){
	vector<StationidAndDemand>::const_iterator it = mincostpath.begin();
	vector<StationidAndDemand>::const_iterator itt = mincostpath.begin();
	_startPoint = -1;

	//for(int i=0; i<mincostpath.size(); i++){
	//	cout<<i<<":"<<mincostpath[i].stationId<<"("<<mincostpath[i].stationDemand<<") ";
	//	if(i%5 == 0){
	//		cout<<endl;
	//	}
	//}
	//cout<<endl;
	//printTempSuperNodeInformation();
	deleteRepeatStationPoint(mincostpath);
	if (checkSum(mincostpath)){
		for (int i=0;i<mincostpath.size();i++)
		{
			int tempSum = mincostpath[i].stationDemand;
			int tempNum = i;
			bool flag = false;

			vector<StationidAndDemand> temppath;
			for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
				temppath.push_back(*it);
			}

			while (tempSum >= 0 && tempSum <= _tspBase.VEHICLE_CAPACITY)
			{
				if (tempNum == i && flag == true) {
					//cout << "�ν�" << endl;
					_startPoint = i;
					revertPath(temppath);
					//if (capacityFlag == true){
					//	tryToMeetPositive(temppath);
					//	tryToMeetNegative(temppath);
					//}
					mapPath(temppath);
					_pathSet.push_back(temppath);

					int costSum = getFinalSum(temppath);
					if (costSum < minSum){
						resultpath.clear();
						for (vector<StationidAndDemand>::iterator it = temppath.begin(); it < temppath.end(); ++it){
							resultpath.push_back(*it);
						}
						minSum = costSum;
						_finalSuperNodeVector_PIECE_P.clear();
						_finalSuperNodeVector_PIECE_N.clear();
						_finalSuperNodeVector_PIECE_0.clear();
						for (int i = 0; i < _superNodeVector_PIECE_P.size(); i++){
							_finalSuperNodeVector_PIECE_P.push_back(_superNodeVector_PIECE_P[i]);
						}
						for (int i = 0; i < _superNodeVector_PIECE_N.size(); i++){
							_finalSuperNodeVector_PIECE_N.push_back(_superNodeVector_PIECE_N[i]);
						}
						for (int i = 0; i < _superNodeVector_PIECE_0.size(); i++){
							_finalSuperNodeVector_PIECE_0.push_back(_superNodeVector_PIECE_0[i]);
						}
					}
					break;
				}
				if (flag == false){
					flag = true;
				}
				tempNum = (tempNum + 1) % temppath.size();
				tempSum = tempSum + temppath[tempNum].stationDemand;
			}
			if (tempSum > _tspBase.VEHICLE_CAPACITY){
				cout << "i" << i << ":" << tempSum << ":" << tempNum << "   ";
			}
		}
		//cout << endl;
	}

	return _startPoint;
}

bool  BspBase::checkSum(vector<StationidAndDemand> &mincostpath){
	int sum = 0;
	for (int i = 0; i < mincostpath.size(); i++){
		sum += mincostpath[i].stationDemand;
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
		mincostpath.push_back(*it);
	}
}

void BspBase::revertPath(vector<StationidAndDemand> &mincostpath){

	for (int i = 0; i < _startPoint; i++){
		StationidAndDemand temp;
		temp.stationId = (*mincostpath.begin()).stationId;
		temp.stationDemand = (*mincostpath.begin()).stationDemand;
		mincostpath.erase(mincostpath.begin());
		mincostpath.push_back(temp);
	}
}

void BspBase::tryToMeetPositive(vector<StationidAndDemand> &mincostpath){
	vector<bool> visitedflag;
	stack<StationidAndDemand> meetstack;
	vector<StationidAndDemand> temppath;
	int vehiclecapacity = 0;

	for (int i = 0; i < mincostpath.size(); i++){
		visitedflag.push_back(false);
	}

	cout << "try to meeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee:" << endl;

	// set zero:
	for (int i = 0; i < mincostpath.size(); i++){
		int tempcapacity = 0;
		// haven't visited:
		if (visitedflag[i] == false){
			visitedflag[i] = true;
			tempcapacity = vehiclecapacity + mincostpath[i].stationDemand;

			// visited it:
			if (tempcapacity <= _tspBase.VEHICLE_CAPACITY && tempcapacity >= 0){
				vehiclecapacity += mincostpath[i].stationDemand;
				cout << mincostpath[i].stationId << ":" << mincostpath[i].stationDemand << ":" << vehiclecapacity << "  ";
			}
			// have something wrong:
			else if (tempcapacity > _tspBase.VEHICLE_CAPACITY){
				while (!meetstack.empty() && tempcapacity > _tspBase.VEHICLE_CAPACITY){
					vehiclecapacity -= meetstack.top().stationDemand;
					tempcapacity = vehiclecapacity;
					visitedflag[meetstack.top().latterPosition] = false;
					cout << "�߳�����" << meetstack.top().stationId << ":" << meetstack.top().stationDemand << ":" << vehiclecapacity << "  ";
					meetstack.pop();
					tempcapacity = vehiclecapacity + mincostpath[i].stationDemand;
				}
				vehiclecapacity += mincostpath[i].stationDemand;
				cout << mincostpath[i].stationId << ":" << mincostpath[i].stationDemand << ":" << vehiclecapacity << "  ";
			}
			else{
				cout << "cao123456" << endl;
			}
		}

		// try to meet:
		for (int j = i + 1; j < mincostpath.size(); j++){
			int tempcapacity = 0;
			if (visitedflag[j] == false && mincostpath[i].stationId == mincostpath[j].stationId && mincostpath[i].stationDemand > 0){
				tempcapacity = vehiclecapacity + mincostpath[j].stationDemand;
				if (tempcapacity <= _tspBase.VEHICLE_CAPACITY){
					vehiclecapacity += mincostpath[j].stationDemand;
					visitedflag[j] = true;
					mincostpath[j].formerPosition = i;
					mincostpath[j].latterPosition = j;
					meetstack.push(mincostpath[j]);
					cout << "�Ž�ȥ" << mincostpath[j].stationId << ":" << mincostpath[j].stationDemand << ":" << vehiclecapacity << "  ";
				}
				else{
					break;
				}
			}
		}
	}
	cout << endl;

	// set meeted station:
	for (int i = 0; i < meetstack.size(); i++){
		mincostpath[meetstack.top().formerPosition].stationDemand += mincostpath[meetstack.top().latterPosition].stationDemand;
		mincostpath[meetstack.top().latterPosition].stationDemand = 0;
		meetstack.pop();
	}

	// delete zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		if ((*it).stationDemand != 0){
			StationidAndDemand temp;
			temp.stationId = (*it).stationId;
			temp.stationDemand = (*it).stationDemand;
			temppath.push_back(temp);
		}
	}

	mincostpath.clear();
	for (vector<StationidAndDemand>::iterator it = temppath.begin(); it < temppath.end(); ++it){
		mincostpath.push_back(*it);
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
	vector<bool> visitedflag;
	stack<StationidAndDemand> meetstack;
	vector<StationidAndDemand> temppath;
	int vehiclecapacity = 0;

	for (int i = 0; i < mincostpath.size(); i++){
		visitedflag.push_back(false);
	}

	// set zero:
	for (int i = 0; i < mincostpath.size(); i++){
		int tempcapacity = 0;
		// haven't visited:
		if (visitedflag[i] == false){
			visitedflag[i] = true;
			tempcapacity = vehiclecapacity + mincostpath[i].stationDemand;
			// visited it:
			if (tempcapacity <= _tspBase.VEHICLE_CAPACITY && tempcapacity >= 0){
				vehiclecapacity += mincostpath[i].stationDemand;
			}
			// have something wrong:
			else if (tempcapacity < 0){
				while (!meetstack.empty() && tempcapacity < 0){
					vehiclecapacity -= meetstack.top().stationDemand;
					tempcapacity = vehiclecapacity;
					visitedflag[meetstack.top().latterPosition] = false;
					meetstack.pop();
					tempcapacity = vehiclecapacity + mincostpath[i].stationDemand;
				}
				vehiclecapacity += mincostpath[i].stationDemand;
			}
			else{
				cout << "cao123456" << endl;
			}
		}

		// try to meet:
		for (int j = i + 1; j < mincostpath.size(); j++){
			int tempcapacity = 0;
			if (visitedflag[j] == false && mincostpath[i].stationId == mincostpath[j].stationId && mincostpath[i].stationDemand < 0){
				tempcapacity = vehiclecapacity + mincostpath[j].stationDemand;
				if (tempcapacity >= 0){
					vehiclecapacity += mincostpath[j].stationDemand;
					visitedflag[j] = true;
					mincostpath[j].formerPosition = i;
					mincostpath[j].latterPosition = j;
					meetstack.push(mincostpath[j]);
				}
				else{
					break;
				}
			}
		}
	}

	// set meeted station:
	for (int i = 0; i < meetstack.size(); i++){
		mincostpath[meetstack.top().formerPosition].stationDemand += mincostpath[meetstack.top().latterPosition].stationDemand;
		mincostpath[meetstack.top().latterPosition].stationDemand = 0;
		meetstack.pop();
	}

	// delete zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		if ((*it).stationDemand != 0){
			StationidAndDemand temp;
			temp.stationId = (*it).stationId;
			temp.stationDemand = (*it).stationDemand;
			temppath.push_back(temp);
		}
	}

	mincostpath.clear();
	for (vector<StationidAndDemand>::iterator it = temppath.begin(); it < temppath.end(); ++it){
		mincostpath.push_back(*it);
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

void BspBase::mapPath(vector<StationidAndDemand> &mincostpath){
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); it++){
		(*it).stationId = _tspBase._mapPartToAll[(*it).stationId];
	}
}

int BspBase::getFinalSum(vector<StationidAndDemand> temp){
	FullGraph::Node unode, vnode;
	int uid,vid;
	int i = 0;
	int tempsum = 0;
	int depotA = 0, depotB = 0;

	vector<StationidAndDemand>::iterator it = temp.begin();
	StationidAndDemand start = temp[0];
	StationidAndDemand end = temp[temp.size() - 1];
	depotA = _tspBase._cost[_tspBase._allStationNum][start.stationId];
	depotB = _tspBase._cost[end.stationId][_tspBase._allStationNum];
	tempsum += depotA + depotB;

	uid = (*it++).stationId;
	unode = (*_tspBase.g)(uid);
	for (; it < temp.end(); ++it){
		vid  = (*it).stationId;
		vnode = (*_tspBase.g)(vid);
		if (vnode != unode){
			tempsum += _tspBase._cost[uid][vid];
		}
		unode = vnode;
		uid = vid;
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

void BspBase::printTempSuperNodeInformation(){
	int num = _superNodeVector_PIECE_P.size();
	cout << "P pieces:" << num << endl;
	for (int i = 0; i < num; i++){
		cout <<"  "<< i << ": Matching " << _superNodeVector_PIECE_P[i]._matchingNumber<<": ";
		for (int j = 0; j < _superNodeVector_PIECE_P[i]._stationidAndDemand.size(); j++){
			cout << _superNodeVector_PIECE_P[i]._stationidAndDemand[j].stationId << "(";
			cout << _superNodeVector_PIECE_P[i]._stationidAndDemand[j].stationDemand << ") ";
		}
		cout << endl;
	}

	num = _superNodeVector_PIECE_N.size();
	cout << "N pieces:" << num << endl;
	for (int i = 0; i < num; i++){
		cout << "  " << i << ": Matching " << _superNodeVector_PIECE_N[i]._matchingNumber << "; ";
		for (int j = 0; j < _superNodeVector_PIECE_N[i]._stationidAndDemand.size(); j++){
			cout << _superNodeVector_PIECE_N[i]._stationidAndDemand[j].stationId << "(";
			cout << _superNodeVector_PIECE_N[i]._stationidAndDemand[j].stationDemand << ") ";
		}
		cout << endl;
	}

	num = _superNodeVector_PIECE_0.size();
	cout << "Zero pieces:" << num << endl;
	for (int i = 0; i < num; i++){
		cout << "  " << i << ": ";
		for (int j = 0; j < _superNodeVector_PIECE_0[i]._stationidAndDemand.size(); j++){
			cout << _superNodeVector_PIECE_0[i]._stationidAndDemand[j].stationId << "(";
			cout << _superNodeVector_PIECE_0[i]._stationidAndDemand[j].stationDemand << ") ";
		}
		cout << endl;
	}
	
}

void BspBase::printSuperNodeInformation(){
	int num = _finalSuperNodeVector_PIECE_P.size();
	cout << "P pieces:" << num << endl;
	for (int i = 0; i < num; i++){
		cout <<"  "<< i << ": Matching " << _finalSuperNodeVector_PIECE_P[i]._matchingNumber<<": ";
		for (int j = 0; j < _finalSuperNodeVector_PIECE_P[i]._stationidAndDemand.size(); j++){
			cout << _finalSuperNodeVector_PIECE_P[i]._stationidAndDemand[j].stationId << "(";
			cout << _finalSuperNodeVector_PIECE_P[i]._stationidAndDemand[j].stationDemand << ") ";
		}
		cout << endl;
	}

	num = _finalSuperNodeVector_PIECE_N.size();
	cout << "N pieces:" << num << endl;
	for (int i = 0; i < num; i++){
		cout << "  " << i << ": Matching " << _finalSuperNodeVector_PIECE_N[i]._matchingNumber << "; ";
		for (int j = 0; j < _finalSuperNodeVector_PIECE_N[i]._stationidAndDemand.size(); j++){
			cout << _finalSuperNodeVector_PIECE_N[i]._stationidAndDemand[j].stationId << "(";
			cout << _finalSuperNodeVector_PIECE_N[i]._stationidAndDemand[j].stationDemand << ") ";
		}
		cout << endl;
	}

	num = _finalSuperNodeVector_PIECE_0.size();
	cout << "Zero pieces:" << num << endl;
	for (int i = 0; i < num; i++){
		cout << "  " << i << ": ";
		for (int j = 0; j < _finalSuperNodeVector_PIECE_0[i]._stationidAndDemand.size(); j++){
			cout << _finalSuperNodeVector_PIECE_0[i]._stationidAndDemand[j].stationId << "(";
			cout << _finalSuperNodeVector_PIECE_0[i]._stationidAndDemand[j].stationDemand << ") ";
		}
		cout << endl;
	}
	
}

void BspBase::printTempPath(vector<StationidAndDemand> temp){
	int sum = 0;
	cout << "size:" << temp.size() << endl;
	for (int i = 0; i < temp.size(); i++){
		cout << temp[i].stationId << "(" << temp[i].stationDemand << ") ";
		sum += temp[i].stationDemand;
	}
	cout << "sum: " << sum << endl;
	cout << "cost: "<<getFinalSum(temp) <<endl;
	cout << endl;
}
