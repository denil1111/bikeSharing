 #include"capacitated.h"



CapacitatedBSP::CapacitatedBSP(TspBase &tspbase):BspBase(tspbase){
	//_tspBase = tspbase;

	//_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	//_superNodeNumber = 0;
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



void CapacitatedBSP::beginPositive(int positivesupernode, int negativesupernode){
	int first;
	int negativebegin = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[0].stationId;
	int negativeend = _superNodeVector_PIECE_N[negativesupernode]._negativeEnd;
	FullGraph::Node beginnode, endnode, unode, vnode;
	beginnode = (*_tspBase.g)(negativebegin);
	endnode = (*_tspBase.g)(negativeend);

	int min = MM;
	int disA, disB, disC = 0;

	if (positivesupernode < _superNodeNumber_PIECE_P - 1){
		vector<StationidAndDemand>::iterator c = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1;
		vector<StationidAndDemand>::iterator d = _superNodeVector_PIECE_P[positivesupernode + 1]._stationidAndDemand.begin();
		FullGraph::Node mnode, nnode;
		mnode = (*_tspBase.g)((*c).stationId);
		nnode = (*_tspBase.g)((*d).stationId);
		if (mnode != nnode){
			disC = (*_tspBase.cost)[(*_tspBase.g).edge(mnode, nnode)];
		}
		else{
			disC = 0;
		}
	}

	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin();
		it < _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1; ++it){
		unode = (*_tspBase.g)((*it).stationId);
		vnode = (*_tspBase.g)((*(it + 1)).stationId);
		disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
		disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
		if (disA + disB + disC < min){
			min = disA + disB;
			first = (*it).stationId;
		}
	}
	
	unode = (*_tspBase.g)(_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.size() - 1].stationId);
	vnode = (*_tspBase.g)(_superNodeVector_PIECE_P[positivesupernode]._positiveNext);
	disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
	disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
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
	beginnode = (*_tspBase.g)(positivebegin);
	endnode = (*_tspBase.g)(positivebegin);

	int min = MM;
	int disA, disB;
	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin();
		it < _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1; ++it){
		unode = (*_tspBase.g)((*it).stationId);
		vnode = (*_tspBase.g)((*(it + 1)).stationId);
		disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
		disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
		if (disA + disB < min){
			min = disA + disB;
			second = (*it).stationId;
		}
	}

	unode = (*_tspBase.g)(_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.size() - 1].stationId);
	vnode = (*_tspBase.g)(_superNodeVector_PIECE_N[negativesupernode]._negativeNext);
	disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
	disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
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
	beginnode = (*_tspBase.g)(negativebegin);
	endnode = (*_tspBase.g)(negativeend);

	int min = MM;
	int disA, disB;
	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1;
		it > _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin() ; --it){
		unode = (*_tspBase.g)((*it).stationId);
		vnode = (*_tspBase.g)((*(it - 1)).stationId);
		disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
		disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
		if (disA + disB < min){
			min = disA + disB;
			first = (*it).stationId;
		}
	}

	unode = (*_tspBase.g)(_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[0].stationId);
	vnode = (*_tspBase.g)(_superNodeVector_PIECE_P[positivesupernode]._positiveReverseNext);
	disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
	disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
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
	beginnode = (*_tspBase.g)(positivebegin);
	endnode = (*_tspBase.g)(positiveend);

	int min = MM;
	int disA, disB;
	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1;
		it > _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin(); --it){
		unode = (*_tspBase.g)((*it).stationId);
		vnode = (*_tspBase.g)((*(it - 1)).stationId);
		disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
		disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
		if (disA + disB < min){
			min = disA + disB;
			second = (*it).stationId;
		}
	}

	unode = (*_tspBase.g)(_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[0].stationId);
	vnode = (*_tspBase.g)(_superNodeVector_PIECE_N[negativesupernode]._negativeReverseNext);
	disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
	disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
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


	if (_startFromWhichPiece == PIECE_N){
		//cout << "calculateFromWhichPiece:PIECE_N" << endl;
		for (int i = 0; i < _superNodeNumber / 2; i++){
			for (int j = 0; j < _superNodeNumber / 2; j++){
				beginNegative(i, j);
			}
		}
		machingSuperNode();
		getPathBeginNegative();

		initMinCostAmongSuperNode();
		for (int i = 0; i < _superNodeNumber / 2; i++){
			for (int j = 0; j < _superNodeNumber / 2; j++){
				beginPositiveReverse(i, j);
			}
		}
		machingSuperNode();
		getPathBeginPositiveReverse();
	}
	else{
		//cout << "calculateFromWhichPiece:PIECE_P" << endl;
		for (int i = 0; i < _superNodeNumber / 2; i++){
			for (int j = 0; j < _superNodeNumber / 2; j++){
				beginPositive(i, j);
			}
		}
		machingSuperNode();
		getPathBeginPositive();

		initMinCostAmongSuperNode();
		for (int i = 0; i < _superNodeNumber / 2; i++){
			for (int j = 0; j < _superNodeNumber / 2; j++){
				beginNegativeReverse(i, j);
			}
		}
		machingSuperNode();
		getPathBeginNegativeReverse();
	}
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
//		unode = (*_tspBase.g)(*positiveit);
//		vnode = (*_tspBase.g)(*negativeit);
//		if (unode == vnode){
//			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
//		}
//		else{
//			_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)];
//		}
//
//		negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
//		for (int j = 0; j < _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode(); j++){
//			// FullGraph::NodeIt u(*_tspBase.g); (*_tspBase.cost)[(*_tspBase.g).edge(v, u)]
//			// use node id get node,
//			if (*positiveit == *negativeit){
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].cost = 0;
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt = positiveit;
//				_minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt = negativeit;
//				continue;
//			}
//			vnode = (*_tspBase.g)(*negativeit);
//			int currentcost;
//			if ((currentcost = (*_tspBase.cost)[(*_tspBase.g).edge(unode, vnode)]) < _minCostAmongSuperNode[positivesupernode][negativesupernode].cost){
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
	//cout << "Path set size:" << _pathSet.size() << endl;

}


