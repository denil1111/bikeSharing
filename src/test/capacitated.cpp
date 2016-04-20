 #include"capacitated.h"



CapacitatedBSP::CapacitatedBSP(TspBase &tspbase){
	_tspbase = tspbase;

	_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_minSum = MM;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
}

CapacitatedBSP::~CapacitatedBSP(){

}

// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void CapacitatedBSP::getTspTour(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	TSP alg(*_tspbase.g, *_tspbase.cost);
	_tspbase._tspSum = alg.run();

	std::vector<Node> vec;
	alg.tourNodes(std::back_inserter(vec));
	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		FullGraph::Node node = *it;
		_tspbase._path.push_back((*_tspbase.g).index(node));
	}

	PRINTFTspPath
}

void CapacitatedBSP::getSuperNodePieces(int number){
	int startstation;
	// used for opposite direction:
	int previousupernode = PIECE_0;
	int previouszeronumber = 0;
	// used for forward direction to record last one super node:
	int lastsupernodeflag = PIECE_NULL;

	vector<int>::iterator it = _tspbase._path.begin();
	for (int i = 0; i < number; i++){
		if (++it == _tspbase._path.end()){
			it = _tspbase._path.begin();
		}
	}
	startstation = *it;

	//cout << "Start station id:" << startstation << " "<<_stationDemand[*it] <<endl << endl;
	int surplusdemand = 0;

	int flag = true;
	int stopflag = 0;
	while (stopflag < _tspbase._stationNum){
		flag = false;

		SuperNode newpiece(_tspbase.VEHICLE_CAPACITY / 2);
		surplusdemand = newpiece.getASuperNode(_tspbase._path, _tspbase._stationDemand, startstation, it, surplusdemand, stopflag, _tspbase._stationNum);

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

void CapacitatedBSP::initMinCostAmongSuperNode(){
	_minCostAmongSuperNode.clear();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		vector<MinCostOfTwoSuperNode> m;
		for (int j = 0; j < _superNodeNumber / 2; j++){
			MinCostOfTwoSuperNode minCostOfTwoSuperNode;
			minCostOfTwoSuperNode.cost = M;
			minCostOfTwoSuperNode.firstNodeIt = _tspbase._path.end();
			minCostOfTwoSuperNode.secondNodeIt = _tspbase._path.end();
			m.push_back(minCostOfTwoSuperNode);
		}
		_minCostAmongSuperNode.push_back(m);
	}
}

void CapacitatedBSP::initSuperNode(){
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

void CapacitatedBSP::beginPositive(int positivesupernode, int negativesupernode){
	int first;
	int negativebegin = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[0].stationId;
	int negativeend = _superNodeVector_PIECE_N[negativesupernode]._negativeEnd;
	FullGraph::Node beginnode, endnode, unode, vnode;
	beginnode = (*_tspbase.g)(negativebegin);
	endnode = (*_tspbase.g)(negativeend);

	int min = MM;
	int disA, disB, disC = 0;

	if (positivesupernode < _superNodeNumber_PIECE_P - 1){
		vector<StationidAndDemand>::iterator c = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1;
		vector<StationidAndDemand>::iterator d = _superNodeVector_PIECE_P[positivesupernode + 1]._stationidAndDemand.begin();
		FullGraph::Node mnode, nnode;
		mnode = (*_tspbase.g)((*c).stationId);
		nnode = (*_tspbase.g)((*d).stationId);
		if (mnode != nnode){
			disC = (*_tspbase.cost)[(*_tspbase.g).edge(mnode, nnode)];
		}
		else{
			disC = 0;
		}
	}

	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin();
		it < _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1; ++it){
		unode = (*_tspbase.g)((*it).stationId);
		vnode = (*_tspbase.g)((*(it + 1)).stationId);
		disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
		disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
		if (disA + disB + disC < min){
			min = disA + disB;
			first = (*it).stationId;
		}
	}
	
	unode = (*_tspbase.g)(_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.size() - 1].stationId);
	vnode = (*_tspbase.g)(_superNodeVector_PIECE_P[positivesupernode]._positiveNext);
	disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
	disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
	if (disA + disB + disC < min){
		min = disA + disB;
		first = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.size() - 1].stationId;
	}

	_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = min;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].first = first;

}

void CapacitatedBSP::beginNegative(int positivesupernode, int negativesupernode){
	int second;
	int positivebegin = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[0].stationId;
	int positiveend = _superNodeVector_PIECE_P[positivesupernode]._positiveEnd;
	FullGraph::Node beginnode, endnode, unode, vnode;
	beginnode = (*_tspbase.g)(positivebegin);
	endnode = (*_tspbase.g)(positivebegin);

	int min = MM;
	int disA, disB;
	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin();
		it < _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1; ++it){
		unode = (*_tspbase.g)((*it).stationId);
		vnode = (*_tspbase.g)((*(it + 1)).stationId);
		disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
		disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
		if (disA + disB < min){
			min = disA + disB;
			second = (*it).stationId;
		}
	}

	unode = (*_tspbase.g)(_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.size() - 1].stationId);
	vnode = (*_tspbase.g)(_superNodeVector_PIECE_N[negativesupernode]._negativeNext);
	disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
	disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
	if (disA + disB < min){
		min = disA + disB;
		second = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.size() - 1].stationId;
	}

	_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = min;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].second = second;
}

void CapacitatedBSP::beginPositiveReverse(int positivesupernode, int negativesupernode){
	int first;
	int negativebegin = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.size() - 1].stationId;
	int negativeend = _superNodeVector_PIECE_N[negativesupernode]._negativeReverseEnd;
	FullGraph::Node beginnode, endnode, unode, vnode;
	beginnode = (*_tspbase.g)(negativebegin);
	endnode = (*_tspbase.g)(negativeend);

	int min = MM;
	int disA, disB;
	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1;
		it > _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin() ; --it){
		unode = (*_tspbase.g)((*it).stationId);
		vnode = (*_tspbase.g)((*(it - 1)).stationId);
		disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
		disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
		if (disA + disB < min){
			min = disA + disB;
			first = (*it).stationId;
		}
	}

	unode = (*_tspbase.g)(_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[0].stationId);
	vnode = (*_tspbase.g)(_superNodeVector_PIECE_P[positivesupernode]._positiveReverseNext);
	disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
	disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
	if (disA + disB < min){
		min = disA + disB;
		first = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[0].stationId;
	}

	_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = min;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].first = first;

}

void CapacitatedBSP::beginNegativeReverse(int positivesupernode, int negativesupernode){
	int second;
	int positivebegin = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.size() - 1].stationId;
	int positiveend = _superNodeVector_PIECE_P[positivesupernode]._positiveReverseEnd;
	FullGraph::Node beginnode, endnode, unode, vnode;
	beginnode = (*_tspbase.g)(positivebegin);
	endnode = (*_tspbase.g)(positiveend);

	int min = MM;
	int disA, disB;
	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1;
		it > _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin(); --it){
		unode = (*_tspbase.g)((*it).stationId);
		vnode = (*_tspbase.g)((*(it - 1)).stationId);
		disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
		disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
		if (disA + disB < min){
			min = disA + disB;
			second = (*it).stationId;
		}
	}

	unode = (*_tspbase.g)(_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[0].stationId);
	vnode = (*_tspbase.g)(_superNodeVector_PIECE_N[negativesupernode]._negativeReverseNext);
	disA = (*_tspbase.cost)[(*_tspbase.g).edge(unode, beginnode)];
	disB = (*_tspbase.cost)[(*_tspbase.g).edge(vnode, endnode)];
	if (disA + disB < min){
		min = disA + disB;
		second = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[0].stationId;
	}

	_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = min;
	_minCostAmongSuperNode[positivesupernode][negativesupernode].second = second;

}

//// new:
//void CapacitatedBSP::calculateMinCostOfTwoSuperNode(int positivesupernode, int negativesupernode){
//	if (_startFromWhichPiece == PIECE_P){
//		cout << "_startFromWhichPiece:PIECE_P" << endl;
//		getPathBeginPositive();
//		getPathBeginNegativeReverse();
//	}
//}

void CapacitatedBSP::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();

	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			beginPositive(i, j);
		}
	}
	machingSuperNode();
	getPathBeginPositive();


	//if (_startFromWhichPiece == PIECE_N){
	//	//cout << "calculateFromWhichPiece:PIECE_N" << endl;
	//	for (int i = 0; i < _superNodeNumber / 2; i++){
	//		for (int j = 0; j < _superNodeNumber / 2; j++){
	//			beginNegative(i, j);
	//		}
	//	}
	//	machingSuperNode();
	//	getPathBeginNegative();

	//	initMinCostAmongSuperNode();
	//	for (int i = 0; i < _superNodeNumber / 2; i++){
	//		for (int j = 0; j < _superNodeNumber / 2; j++){
	//			beginPositiveReverse(i, j);
	//		}
	//	}
	//	machingSuperNode();
	//	getPathBeginPositiveReverse();
	//}
	//else{
	//	//cout << "calculateFromWhichPiece:PIECE_P" << endl;
	//	for (int i = 0; i < _superNodeNumber / 2; i++){
	//		for (int j = 0; j < _superNodeNumber / 2; j++){
	//			beginPositive(i, j);
	//		}
	//	}
	//	machingSuperNode();
	//	getPathBeginPositive();

	//	initMinCostAmongSuperNode();
	//	for (int i = 0; i < _superNodeNumber / 2; i++){
	//		for (int j = 0; j < _superNodeNumber / 2; j++){
	//			beginNegativeReverse(i, j);
	//		}
	//	}
	//	machingSuperNode();
	//	getPathBeginNegativeReverse();
	//}
}

//void CapacitatedBSP::calculateMinCostOfTwoSuperNode(int positivesupernode, int negativesupernode){
//	// firstit and secondit point to _path.
//	vector<int>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode].getStartIt();
//	vector<int>::iterator negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
//	FullGraph::Node unode, vnode;
//	_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt = positiveit;
//	_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt = negativeit;
//
//	for (int i = 0; i < _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode(); i++){
//		unode = (*_tspbase.g)(*positiveit);
//		vnode = (*_tspbase.g)(*negativeit);
//		if (unode == vnode){
//			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
//		}
//		else{
//			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = (*_tspbase.cost)[(*_tspbase.g).edge(unode, vnode)];
//		}
//
//		negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
//		for (int j = 0; j < _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode(); j++){
//			// FullGraph::NodeIt u(*_tspbase.g); (*_tspbase.cost)[(*_tspbase.g).edge(v, u)]
//			// use node id get node,
//			if (*positiveit == *negativeit){
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt = positiveit;
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt = negativeit;
//				continue;
//			}
//			vnode = (*_tspbase.g)(*negativeit);
//			int currentcost;
//			if ((currentcost = (*_tspbase.cost)[(*_tspbase.g).edge(unode, vnode)]) < _minCostAmongSuperNode[positivesupernode][negativesupernode].cost){
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = currentcost;
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt = positiveit;
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt = negativeit;
//			}
//			if (++negativeit == _path.end()){
//				negativeit = _path.begin();
//			}
//		}
//		if (++positiveit == _path.end()){
//			positiveit = _path.begin();
//		}
//	}
//	return;
//}

//void CapacitatedBSP::calculateMinCostAmongSuperNode(){
//	initMinCostAmongSuperNode();
//	for (int i = 0; i < _superNodeNumber / 2; i++){
//		for (int j = 0; j < _superNodeNumber / 2; j++){
//			calculateMinCostOfTwoSuperNode(i, j);
//		}
//	}
//}

string CapacitatedBSP::getLGF(){
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

void CapacitatedBSP::machingSuperNode(){
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

void CapacitatedBSP::pushbackStationidAndDemand(vector<StationidAndDemand> &tempVector, StationidAndDemand temp){
	int num = tempVector.size();
	if (num == 0){
		tempVector.push_back(temp);
	}
	else{
		tempVector.push_back(temp);
	}
}

void CapacitatedBSP::getPositivePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	pushbackStationidAndDemand(tempVector, *itt);
}

void CapacitatedBSP::getPositivePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_P[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void CapacitatedBSP::getNegativePathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	pushbackStationidAndDemand(tempVector, *itt);
}

void CapacitatedBSP::getNegativePath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_N[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void CapacitatedBSP::getZeroPathReverse(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end() - 1;
	for (itt; itt > _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin(); --itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
	pushbackStationidAndDemand(tempVector, *itt);
}

void CapacitatedBSP::getZeroPath(int currentnumberofzeropiece, vector<StationidAndDemand> &tempVector){
	vector<StationidAndDemand>::iterator itt = _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.begin();
	for (itt; itt < _superNodeVector_PIECE_0[currentnumberofzeropiece]._stationidAndDemand.end(); ++itt){
		pushbackStationidAndDemand(tempVector, *itt);
	}
}

void CapacitatedBSP::getZeroPathInBehind(vector<StationidAndDemand> &tempVector){
	int zeronum = _superNodeVector_PIECE_0.size();
	int i = zeronum - 1;
	for (int j = 0; j < _zeroSuperNodeNumberInEnd; j++, i--){
		getZeroPathReverse(i, tempVector);
	}
}

void CapacitatedBSP::getZeroPathInFront(vector<StationidAndDemand> &tempVector){
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPath(i, tempVector);
	}
}

void CapacitatedBSP::getPathBeginPositiveReverse(){
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

void CapacitatedBSP::getPathBeginPositive(){
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

void CapacitatedBSP::getPathBeginNegativeReverse(){
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

void CapacitatedBSP::getPathBeginNegative(){
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

//void CapacitatedBSP::getPath(){
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

int CapacitatedBSP::getFinalSum(vector<StationidAndDemand> temp){
	FullGraph::Node unode, vnode;
	int i = 0;
	int tempsum = 0;
	vector<StationidAndDemand>::iterator it = temp.begin();

	unode = (*_tspbase.g)((*it++).stationId);
	for (; it < temp.end() - 1; ++it){
		vnode = (*_tspbase.g)((*it).stationId);
		if (vnode != unode){
			tempsum += (*_tspbase.cost)[(*_tspbase.g).edge(unode, vnode)];
		}
		unode = vnode;
	}

	return tempsum;
}

bool  CapacitatedBSP::checkSum(){
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

int CapacitatedBSP::getStartStationCapacitatedBSP(){
	return _startStationCapacitatedBSP;
}

int CapacitatedBSP::getStartStationCapacitated(vector<StationidAndDemand> &mincostpath){
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

void CapacitatedBSP::revertPath(vector<StationidAndDemand> &mincostpath){
	for (int i = 0; i < _startPoint; i++){
		StationidAndDemand temp;
		temp.isCutPoint = (*mincostpath.begin()).isCutPoint;
		temp.stationId = (*mincostpath.begin()).stationId;
		temp.stationDemand = (*mincostpath.begin()).stationDemand;
		mincostpath.erase(mincostpath.begin());
		mincostpath.push_back(temp);
	}
}

void CapacitatedBSP::tryToMeetPositive(vector<StationidAndDemand> &mincostpath){
	vector<StationidAndDemand> temppath;
	int vehiclecapacity = 0;

	// set zero:
	for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
		vehiclecapacity += (*it).stationDemand;
		if ((*it).isCutPoint == PIECE_P){
			for (vector<StationidAndDemand>::iterator itt = it + 1; itt < mincostpath.end(); ++itt){
				if ((*itt).stationId == (*it).stationId && (*it).stationDemand != 0){
					if (vehiclecapacity + (*itt).stationDemand <= _tspbase.VEHICLE_CAPACITY){
						vehiclecapacity += (*itt).stationDemand;
						(*it).stationDemand += (*itt).stationDemand;
						(*itt).stationDemand = 0;
					}
					else {
						(*it).stationDemand += _tspbase.VEHICLE_CAPACITY - vehiclecapacity;
						(*itt).stationDemand -= _tspbase.VEHICLE_CAPACITY - vehiclecapacity;
						vehiclecapacity = _tspbase.VEHICLE_CAPACITY;
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

void CapacitatedBSP::tryToMeetNegative(vector<StationidAndDemand> &mincostpath){
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
						vehiclecapacity += (*itt).stationDemand;
						(*it).stationDemand += (*itt).stationDemand;
						(*itt).stationDemand = 0;
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

void CapacitatedBSP::mapPath(){
	for (vector<StationidAndDemand>::iterator it = _minCostPath.begin(); it < _minCostPath.end(); it++){
		(*it).stationId = _tspbase._mapPartToAll[(*it).stationId];
	}
}

void CapacitatedBSP::deleteRepeatStationPoint(vector<StationidAndDemand> &mincostpath){
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



void CapacitatedBSP::runRandom(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	totaltime0 = finish - sum;

	cout << endl<<"CapacitatedBSP:" << endl;

	//for (int i = 0; i < VEHICLE_CAPACITY / 2; i++){
	int i = 0;
	initSuperNode();
	getSuperNodePieces(i);
	calculateMinCostAmongSuperNode();
	//machingSuperNode();
	//getPath();
	//cout << "Get a path " << i << endl << endl;
	//}

	PRINTFFinalPath

	cout << "Capacitated BSP sum cost:" << _minSum << endl << endl;
	//cout << "Mininum sum cost:" << _minSum << endl;
	//cout << "StartStation:" << _startStationCapacitatedBSP << endl;
	//cout << "length:" << _minCostPath.size() << endl;
	//cout << "Path set size:" << _pathSet.size() << endl;

}

void CapacitatedBSP::run(){

	_tspbase.data();

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	totaltime0 = finish - sum;

	cout <<endl<<"CapacitatedBSP:" << endl;

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

	cout << "Capacitated BSP sum cost:" << _minSum << endl << endl;
	//cout << "Mininum sum cost:" << _minSum << endl;
	//cout << "StartStation:" << _startStationCapacitatedBSP << endl;
	//cout << "length:" << _minMediumCostPath.size() << endl;
	//cout << "Path set size:" << _pathSet.size() << endl;

}

void CapacitatedBSP::printTspPath(){
	cout << "TSP tour:" << _tspbase._path.size() << endl;
	for (int i = 0; i < _tspbase._path.size(); i++){
		cout << _tspbase._path[i] << "(" << _tspbase._stationDemand[_tspbase._path[i]] << ") ";
	}
	cout << endl;
}

void CapacitatedBSP::printFinalPath(){

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

void CapacitatedBSP::printSuperNodeInformation(){

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

void CapacitatedBSP::printTempPath(vector<StationidAndDemand> temp){
	int sum = 0;
	cout << "size:" << temp.size() << endl;
	for (int i = 0; i < temp.size(); i++){
		cout << temp[i].stationId << "(" << temp[i].stationDemand << ") ";
		sum += temp[i].stationDemand;
	}
	cout << "sum: " << sum << endl;
	cout << endl;
}
