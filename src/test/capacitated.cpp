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
	int disA, disB, disC = 0;

	if (negativesupernode < _superNodeNumber_PIECE_N - 1){
		vector<StationidAndDemand>::iterator c = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1;
		vector<StationidAndDemand>::iterator d = _superNodeVector_PIECE_N[negativesupernode + 1]._stationidAndDemand.begin();
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

	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin();
		it < _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.end() - 1; ++it){
		unode = (*_tspBase.g)((*it).stationId);
		vnode = (*_tspBase.g)((*(it + 1)).stationId);
		disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
		disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
		if (disA + disB + disC < min){
			min = disA + disB + disC;
			second = (*it).stationId;
		}
	}

	unode = (*_tspBase.g)(_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand[_superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.size() - 1].stationId);
	vnode = (*_tspBase.g)(_superNodeVector_PIECE_N[negativesupernode]._negativeNext);
	disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
	disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
	if (disA + disB + disC < min){
		min = disA + disB + disC;
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
	int disA, disB, disC = 0;

	if (positivesupernode > 1){
		vector<StationidAndDemand>::iterator c = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin();
		vector<StationidAndDemand>::iterator d = _superNodeVector_PIECE_P[positivesupernode - 1]._stationidAndDemand.end() - 1;
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

	for (vector<StationidAndDemand>::iterator it = _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.end() - 1;
		it > _superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand.begin() ; --it){
		unode = (*_tspBase.g)((*it).stationId);
		vnode = (*_tspBase.g)((*(it - 1)).stationId);
		disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
		disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
		if (disA + disB + disC < min){
			min = disA + disB + disC;
			first = (*it).stationId;
		}
	}

	unode = (*_tspBase.g)(_superNodeVector_PIECE_P[positivesupernode]._stationidAndDemand[0].stationId);
	vnode = (*_tspBase.g)(_superNodeVector_PIECE_P[positivesupernode]._positiveReverseNext);
	disA = (*_tspBase.cost)[(*_tspBase.g).edge(unode, beginnode)];
	disB = (*_tspBase.cost)[(*_tspBase.g).edge(vnode, endnode)];
	if (disA + disB + disC < min){
		min = disA + disB + disC;
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
	int disA, disB, disC = 0;

	if (negativesupernode > 1){
		vector<StationidAndDemand>::iterator c = _superNodeVector_PIECE_N[negativesupernode]._stationidAndDemand.begin();
		vector<StationidAndDemand>::iterator d = _superNodeVector_PIECE_N[negativesupernode + 1]._stationidAndDemand.end() - 1;
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

void CapacitatedBSP::calculateMinCostAmongSuperNode(){
	if (_startFromWhichPiece == PIECE_N){
		//cout << "calculateFromWhichPiece:PIECE_N" << endl;
		initMinCostAmongSuperNode();
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
		initMinCostAmongSuperNode();
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

void CapacitatedBSP::run(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

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
	deleteRepeatStationPoint(_minCostPath);
	getStartStation(_minCostPath);
	revertPath(_minCostPath);
	mapPath();

	PRINTFFinalPath

	cout << "Capacitated BSP sum cost:" << _minSum << endl << endl;
}


