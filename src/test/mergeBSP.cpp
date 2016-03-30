 #include"mergeBSP.h"


MergeBSP::MergeBSP(int num){

	// used for UncapacitatedBSP:
	_startStation = -1;

	_stationNum = num;
	_startStationId = -1;
	g = new FullGraph(num);
	cost = new DoubleEdgeMap(*g);
	pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
	_superNodeNumber = 0;
	_finalSum = 0;
	_finalTSPSum = 0;

	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
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
		// 正站点访问标记,如果访问过，就放进这个vector中。如果下次又轮到这个点，说明无解
		vector<int>::iterator result = find(_positiveStationVisiteFlag.begin(), _positiveStationVisiteFlag.end(), *it);
		if (result != _positiveStationVisiteFlag.end()){
			cout << "No solution!!!!!" << endl;
			break;
		}

		// 这是一次寻找的开始，第一个点的需求一定要大于等于0才行
		if (isAPositiveStation(*it)){
			_positiveStationVisiteFlag.push_back(*it);
			_startStation = *it;
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
					return _startStation;
			}

			// 如果出现了和小于0，那就跳过后面还是负需求的站点跳过：
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


// 
void MergeBSP::getRandomPoints(){
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		point poss;
		poss.a = rand() % POINT_RANGE;
		poss.b = rand() % POINT_RANGE;
		(*pos)[u] = dim2::Point<int>(poss.a, poss.b);
		_point.push_back(poss);
	}
}

void MergeBSP::getPoints(){
	double a, b;
	srand((unsigned)time(NULL));

	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		_point[i].a = rand() % POINT_RANGE;
		_point[i].b = rand() % POINT_RANGE;
		(*pos)[u] = dim2::Point<int>(_point[i].a, _point[i].b);
	}
	PRINTFRandomPoints
}



// random:（ -STATION_CAPACITY，STATION_CAPACITY ）
void MergeBSP::getRandomDemand(){
	int temp;
	int sum = 0;

	srand((unsigned)time(NULL));
	for (int i = 0; i < _stationNum; i++){
		temp = rand() % (STATION_CAPACITY * 2 + 1) - STATION_CAPACITY;
		_stationDemand.push_back(temp);
		sum += temp;
	}
	sum -= temp;
	_stationDemand[_stationNum - 1] = -sum;

	PRINTFRandomDemand
}

void MergeBSP::getDemand(){

}

void MergeBSP::getCost(){
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		int j = 0;
		for (FullGraph::NodeIt v = u; v != INVALID; ++v, ++j) {
			if (u != v) {
				(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = _cost[i][j];
			}
		}
	}
}

void MergeBSP::getRandomCost(){
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		for (FullGraph::NodeIt v = u; v != INVALID; ++v) {
			if (u != v) {
				(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = (int)sqrt(((*pos)[u] - (*pos)[v]).normSquare());
			}
		}
	}
	PRINTFCost
}

int MergeBSP::getStartStationId(){
	return _startStationId;
}

/*
void MergeBSP::getRandomCost(){

int a[5][5] = { { 0, 1, 2, 3, 4 },
{ 1, 0, 4, 5, 7 },
{ 2, 4, 0, 6, 8 },
{ 3, 5, 6, 0, 9 },
{ 4, 7, 8, 9, 0 }
};
int b[3][3] = { { 0, 1, 2 },
{ 3, 0, 4 },
{ 5, 6, 0 }
};
int c[4][4] = { { 0, 4, 2, 1 },
{ 4, 0, 1, 5 },
{ 2, 1, 0, 2 },
{ 1, 5, 2, 0 }
};
int d[5][5] = { { 1, 200, 100, 1, 1 },
{ 200, 1, 1, 1, 1 },
{ 100, 1, 1, 100, 1 },
{ 1, 1, 100, 1, 100 },
{ 1, 1, 1, 100, 1 }
};
int e[5][5] = { { 100, 200, 10000, 100, 100 },
{ 200, 100, 100, 100, 1000 },
{ 10000, 100, 100, 100, 100 },
{ 100, 100, 100, 100, 100 },
{ 1000, 100, 100, 100, 100 }
};
cout << "获取Cost:" << endl;
int i = 0, j = 0;
for (FullGraph::NodeIt u(*g); u != INVALID; ++u, i++) {
j = i;
for (FullGraph::NodeIt v = u; v != INVALID; ++v, j++) {
if (u == v) {
//cout << "cost[" << i << "][" << j << "] = " << 0 << " ";
cout << 0 << " ";
continue;
}
//cost[g.edge(u, v)] = (pos[u] - pos[v]).normSquare();
(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = c[i][j];
//cout << "cost[" << i << "][" << j << "] = " << (*cost)[(*g).edge(u, v)] << " ";
cout << (*cost)[(*g).edge(u, v)] << " ";
}
cout << endl;
}
cout << endl;
}
*/

// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void MergeBSP::getTspTour(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	//TspCheck tspCheck;
	TSP alg(*g, *cost);

	_sum = alg.run();

	//tspCheck.check(alg.run() > 0, alg_name + ": Wrong total cost");

	std::vector<Node> vec;
	alg.tourNodes(std::back_inserter(vec));

	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		FullGraph::Node node = *it;
		_path.push_back((*g).index(node));
	}

	// 下面都是check
	//tspCheck.check(tspCheck.checkTour(*g, vec), alg_name + ": Wrong node sequence");
	//tspCheck.check(tspCheck.checkCost(*g, vec, *cost, alg.tourCost()), alg_name + ": Wrong tour cost");

	PRINTFTSPtour
}

void MergeBSP::getSuperNodePieces(){
	int lastsupernodeflag = PIECE_0;

	vector<int>::iterator it = _path.begin();
	while (_stationDemand[*it] < 0){
		++it;
	}
	_startStationId = *it;
	cout << "起始点：" << _startStationId << endl << endl << endl;
	int surplusdemand = 0;

	bool flag = true;
	while (*it != _startStationId || flag){
		flag = false;
		SuperNode newpiece(Q / 2);
		surplusdemand = newpiece.getASuperNode(_path, _stationDemand, _startStationId, it, surplusdemand);
		if (newpiece.getPieceTypeFlag() == PIECE_P){
			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;
			_superNodeVector_PIECE_P.push_back(newpiece);
			lastsupernodeflag = PIECE_P;
		}
		else if (newpiece.getPieceTypeFlag() == PIECE_N){
			_superNodeNumber_PIECE_N++;
			++_superNodeNumber;
			_superNodeVector_PIECE_N.push_back(newpiece);
			lastsupernodeflag = PIECE_N;
		}
		else{
			_superNodeVector_PIECE_0.push_back(newpiece);
			_superNodeNumber_PIECE_0++;
			if (lastsupernodeflag == PIECE_0){
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
	cout << "calculateMinCostAmongSuperNode" << endl;
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

void MergeBSP::getZeroPathInFront(){
	//cout << "最前面的零块：";
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPathBehindPositive(i);
	}
}

void MergeBSP::getPath(){
	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront();
		currentnumberofzeropiece += _zeroSuperNodeNumberInFront;
	}
	if (_superNodeNumber >= 2){
		for (int positivesupernode = 0; positivesupernode < _superNodeNumber / 2; positivesupernode++){
			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();
			vector<int>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode].getStartIt();
			//cout << "这是一个正块:";
			//cout << " " << _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode() << " ";
			for (int i = 0; i < _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode(); i++, ++positiveit){
				if (positiveit == _path.end()){
					positiveit = _path.begin();
				}
				if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *positiveit) || _finalPaht.size() == 0){
					_finalPaht.push_back(*positiveit);
				}

				//cout << *positiveit << "正 ";
				if (positiveit == _minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt){
					vector<int>::iterator negativestartit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
					vector<int>::iterator negativeendit = _superNodeVector_PIECE_N[negativesupernode].getEndIt();
					vector<int>::iterator nagetiveit = _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt;
					vector<int>::iterator targetit = _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt;
					//cout << "这是一个负块:";
					if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *nagetiveit) || _finalPaht.size() == 0){
						_finalPaht.push_back(*nagetiveit);
					}
					//cout << *nagetiveit << " ";
					if (nagetiveit == negativestartit){

						int numberofzeropiece = 0;
						if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
							for (int i = 0; i < numberofzeropiece; i++){
								getZeroPath(currentnumberofzeropiece);
								++currentnumberofzeropiece;
							}
						}

						nagetiveit = negativeendit;
					}
					else if (nagetiveit == _path.begin()){
						nagetiveit = _path.end() - 1;
					}
					else{
						--nagetiveit;
					}

					while (nagetiveit != targetit){
						if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *nagetiveit) || _finalPaht.size() == 0){
							_finalPaht.push_back(*nagetiveit);
						}
					//	cout << *nagetiveit << "负 ";
						if (nagetiveit == negativestartit){
							nagetiveit = negativeendit;

							int numberofzeropiece = 0;
							if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
								for (int i = 0; i < numberofzeropiece; i++){
									getZeroPath(currentnumberofzeropiece);
									++currentnumberofzeropiece;
								}
							}

						}
						else if (nagetiveit == _path.begin()){
							nagetiveit = _path.end() - 1;
						}
						else{
							--nagetiveit;
						}
					}
					if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *targetit) || _finalPaht.size() == 0){
						_finalPaht.push_back(*targetit);
					}
					//cout << *nagetiveit << "负 ";
					if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *positiveit) || _finalPaht.size() == 0){
						_finalPaht.push_back(*positiveit);
					}
				//	cout << *positiveit << "正 ";
				}// if negative

				int numberofzeropiece = 0;
				if (positiveit == _superNodeVector_PIECE_P[positivesupernode].getEndIt()
					&& (numberofzeropiece = _superNodeVector_PIECE_P[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						getZeroPath(currentnumberofzeropiece);
						++currentnumberofzeropiece;
					}
				}
			}//for positive
			//cout << endl;
		}// for positive and negative
	}// if
//	cout << endl;

	PRINTFFinalPath

}

void MergeBSP::getZeroPath(int currentnumberofzeropiece){
	vector<int>::iterator it = _superNodeVector_PIECE_0[currentnumberofzeropiece].getStartIt();
	//cout << endl << "这是一个零块:" << _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *it) || _finalPaht.size() == 0){
			_finalPaht.push_back(*it);
			
		}
		//cout << *it << "零 ";
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
}

void MergeBSP::getZeroPathBehindPositive(int &currentnumberofzeropiece){
	vector<int>::iterator it = _superNodeVector_PIECE_0[currentnumberofzeropiece].getStartIt();
	//cout << endl << "这是一个零块:" << _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode() << endl;
	for (int i = 0; i < _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *it) || _finalPaht.size() == 0){
			_finalPaht.push_back(*it);
			
		}
		//cout << *it << "零ha ";
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
}

void MergeBSP::getZeroPathReverse(int currentnumberofzeropiece){
	vector<int>::iterator it = _superNodeVector_PIECE_0[currentnumberofzeropiece].getEndIt();
	cout << endl << "回溯一个零块:" << _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode()<<" ";
	for (int i = 0; i < _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((_finalPaht.size() > 0 && *(_finalPaht.end() - 1) != *it) || _finalPaht.size() == 0){
			_finalPaht.push_back(*it);
			
		}
		cout << *it << "零" << i << " ";
		if (--it == _path.begin()){
			it = _path.end() - 1;
		}
	}
}

void MergeBSP::getZeroPathBehindNegative(int &currentnumberofzeropiece){
	int numberofzeropiece = 0;
	int current = currentnumberofzeropiece;
	if ((numberofzeropiece = _superNodeVector_PIECE_N[current].getNumberOfZeroPieceBehind()) != 0){
		for (int i = 0; i < numberofzeropiece; i++){
			getZeroPath(current);
			cout << endl << "苍天大老爷" << current << endl;
			++current;
		}
		for (int i = 0; i < numberofzeropiece; i++){
			--current;
			getZeroPathReverse(current);
		}
		currentnumberofzeropiece += numberofzeropiece;
	}	
}


void MergeBSP::getCompletePath(){
	int currentnumberofzeropiece = 0;

	// get zero super node path in front:
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront();
		currentnumberofzeropiece += _zeroSuperNodeNumberInFront;
	}

	if (_superNodeNumber >= 2){
		cout << "_superNodeNUmber" << _superNodeNumber / 2;
		for (int positivesupernode = 0; positivesupernode < _superNodeNumber / 2; positivesupernode++){

			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();
			vector<int>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode].getStartIt();
			cout << endl << endl << "This is a positive supernode: NO:" << positivesupernode ;
			cout << " Number:" << _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode() << " Endlt:";
			cout << *_superNodeVector_PIECE_P[positivesupernode].getEndIt() << endl;

			for (int i = 0; i < _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode(); i++, ++positiveit){
				if (positiveit == _path.end()){
					positiveit = _path.begin();
				}
				// avoid some adjacent and  equivalent node added into the path:
				if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *positiveit){
					_finalPaht.push_back(*positiveit);
					
				}
				cout << *positiveit << "正 ";
			    
		
				if (positiveit == _minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt){
					cout << endl << "This is a negative supernode:" << _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode() << endl;
					vector<int>::iterator negativestartit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
					vector<int>::iterator negativeendit = _superNodeVector_PIECE_N[negativesupernode].getEndIt();
					vector<int>::iterator nagetiveit = _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt;
					vector<int>::iterator targetit = _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt;

					if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *nagetiveit){
						_finalPaht.push_back(*nagetiveit);
						
					}
					cout << *nagetiveit << "负 ";

					// until to the start node
					while (nagetiveit != negativestartit){
						if (nagetiveit == _path.begin()){
							nagetiveit = _path.end() - 1;
						}
						else{
							--nagetiveit;
						}
						if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *nagetiveit){
							_finalPaht.push_back(*nagetiveit);
							
						}
						cout << *nagetiveit << "负 ";
					}

					if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *nagetiveit){
						_finalPaht.push_back(*nagetiveit);
						
					}
					cout << *nagetiveit << "负 ";

					for (int j = 1; j < _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode(); j++){
						if (++nagetiveit == _path.end()){
							nagetiveit = _path.begin();
						}
						if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *nagetiveit){
							_finalPaht.push_back(*nagetiveit);
							
						}
						cout << *nagetiveit << "负 ";
					}

					if (nagetiveit != negativeendit){
						cout << "Oh My God!!!!!" << endl;
					}

					//
					getZeroPathBehindNegative(currentnumberofzeropiece);
					
					// return back from a zero super node:
					if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *nagetiveit){
						_finalPaht.push_back(*nagetiveit);
						
					}
					cout << *nagetiveit << "负 ";
					
					while (nagetiveit != targetit){
						if (nagetiveit == _path.begin()){
							nagetiveit = _path.end() - 1;
						}
						else{
							--nagetiveit;
						}

						if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *nagetiveit){
							_finalPaht.push_back(*nagetiveit);
							
						}
						cout << *nagetiveit << "负 ";
					}
					if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *nagetiveit){
						_finalPaht.push_back(*targetit);
						
					}
					cout << *nagetiveit << "负 ";
					
					if (_finalPaht.size() == 0 || *(_finalPaht.end() - 1) != *positiveit){
						_finalPaht.push_back(*positiveit);
						
					}
					cout << *positiveit << "正 ";
				}// if negative
				int numberofzeropiece = 0;
				if (positiveit == _superNodeVector_PIECE_P[positivesupernode].getEndIt()
					&& (numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceBehind()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						cout << "caodian" << endl;
						getZeroPathBehindPositive(currentnumberofzeropiece);
						cout << endl << "苍天大老爷" << currentnumberofzeropiece << endl;
						++currentnumberofzeropiece;
					}
				}
			}//for positive
			//cout << endl;
		}// for positive and negative
	}// if
	cout << endl;

	PRINTFFinalPath

}




int MergeBSP::getTSPSum(){
	FullGraph::Node unode, vnode;

	int i = 0;
	vector<int>::iterator it = _path.begin();
	unode = (*g)(*(it++));
	//vnode = (*g)(*negativeit);
	for (; it < _path.end(); ++it){
		vnode = (*g)(*it);
		if (vnode != unode){
			_finalTSPSum += (*cost)[(*g).edge(unode, vnode)];
			cout << (*cost)[(*g).edge(unode, vnode)] << " ";
			if (i++ % 20 == 0){
				cout << endl;
			}
		}
		unode = vnode;
	}

	return _finalTSPSum;
}

int MergeBSP::getFinalSum(){
	FullGraph::Node unode, vnode;

	int i = 0;
	vector<int>::iterator it = _finalPaht.begin();
	unode = (*g)(*(it++));
	//vnode = (*g)(*negativeit);
	for (; it < _finalPaht.end(); ++it){
		vnode = (*g)(*it);
		if (vnode != unode){
			_finalSum += (*cost)[(*g).edge(unode, vnode)];
			cout << (*cost)[(*g).edge(unode, vnode)] << " ";
			if (i++ % 20 == 0){
				cout << endl;
			}
		}
		unode = vnode;
	}
	cout << endl;

	return _finalSum;
}

void MergeBSP::randomData(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	cout << "vehicle capacity:" << VEHICLE_CAPACITY << endl;
	cout << "station capacity:" << STATION_CAPACITY << endl;

	start = clock();
	getRandomPoints();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetRandomPoints:" << totaltime << "ms！" << endl;

	start = clock();
	getRandomCost();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetCost:" << totaltime << "ms！" << endl;

	start = clock();
	getRandomDemand();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetRandomDemand:" << totaltime << "ms！" << endl;

}

void MergeBSP::run(){

	clock_t start, finish, sum;
	double totaltime,totaltime0,totaltime1,totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetTspTour:" << totaltime << "ms！" << endl;

	totaltime0 = finish - sum;

	// Uncapacitated BSP:
	cout << endl << "UncapacitatedBSP:" << endl;
	sum = clock();
	start = clock();
	getStartStation();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetStartStation:" << totaltime << "ms！" << endl;

	totaltime1 = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;
	cout << "\nUncapacitated BSP time:" << totaltime1 << "ms！ Sum cost:" << getTSPSum() << endl << endl;

	// Capacitated BSP:
	cout << endl << "CapacitatedBSP:" << endl;

	sum = clock();
	start = clock();
	getSuperNodePieces();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetSuperNodePieces:" << totaltime << "ms！" << endl;

	start = clock();
	calculateMinCostAmongSuperNode();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ncalculateMinCostAmongSuperNode:" << totaltime << "ms！" << endl;

	start = clock();
	machingSuperNode();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\nmachingSuperNode:" << totaltime << "ms！" << endl;

	start = clock();
	getPath();
	//getCompletePath();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetPath:" << totaltime << "ms！" << endl;

	totaltime2 = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;
	cout << "\nCapacitated BSP time:" << totaltime2 << "ms！ Sum cost:" << getFinalSum() << endl;
	cout << "length:" << _finalPaht.size() << endl;

}

void MergeBSP::printRandomPoints(){
	cout << "Point：" << endl;
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u){
		cout << "(" << (*pos)[u].x << ", " << (*pos)[u].y << ") ";
		if ((i++ + 1) % 10 == 0){
			cout << endl;
		}
	}
	cout << endl << endl;
}

void MergeBSP::printRandomDemand(){
	cout << "Station Demand:" << endl;
	for (int i = 0; i < _stationNum; i++){
		printf("%3d ", _stationDemand[i]);
		if ((i + 1) % 20 == 0){
			cout << endl;
		}
	}
	cout << endl << endl;
}


void MergeBSP::printCost(){
	int i = 0;
	cout << "Cost:" << endl;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		cout << "Node: " << i++ << endl;
		for (FullGraph::NodeIt v(*g); v != INVALID; ++v) {
			if (u == v) {
				cout << "  0 ";
			}
			else{
				cout << (*cost)[(*g).edge(u, v)] << " ";
			}
		}
		cout << endl << endl;
	}
}

void MergeBSP::printTSPtour(){
	int i = 0;
	cout << "TSP tour：" << endl;
	for (vector<int>::iterator it = _path.begin(); it < _path.end(); ++it){
		cout << *it << " ";
		if ((i++ + 1) % 30 == 0){
			cout << endl;
		}
	}
	cout << endl;
	cout << "TSP tour demand sum: " << _sum << endl << endl;
}

void MergeBSP::printFinalPath(){

	cout << "TSP路径：" << _path.size() << endl;
	for (int i = 0; i < _path.size(); i++){
		cout << _path[i] << "(" << _stationDemand[_path[i]] << ") ";
	}
	cout << endl;

	cout << "最终路径：" << _finalPaht.size() << endl;
	for (int i = 0; i < _finalPaht.size(); i++){
		cout << _finalPaht[i] << "(" << _stationDemand[_finalPaht[i]] << ") ";
	}
	cout << endl;
}

void MergeBSP::printSuperNodeInformation(){

	cout << "_superNodeNumber_PIECE_P = " << _superNodeNumber_PIECE_P << endl;
	cout << "_superNodeNumber_PIECE_N = " << _superNodeNumber_PIECE_N << endl;
	cout << "_superNodeNumber_PIECE_0 = " << _superNodeNumber_PIECE_0 << endl;

	cout << "Positive super node：" << endl;
	for (int i = 0; i < _superNodeNumber / 2; i++){
		cout << "startit = " << *_superNodeVector_PIECE_P[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_P[i].getEndIt();
		cout << "  mincostpoint = " << _superNodeVector_PIECE_P[i].getMatchingNumber();
		cout << "  matching point :" << *_minCostAmongSuperNode[i][_superNodeVector_PIECE_P[i].getMatchingNumber()].firstNodeIt << endl;
	}
	cout << "Negative super node：" << endl;
	for (int i = 0; i < _superNodeNumber / 2; i++){
		cout << "startit = " << *_superNodeVector_PIECE_N[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_N[i].getEndIt();
		cout << "  mincostpoint = " << _superNodeVector_PIECE_N[i].getMatchingNumber() << endl;
		cout << "  matching point :" << *_minCostAmongSuperNode[_superNodeVector_PIECE_N[i].getMatchingNumber()][i].secondNodeIt << endl;
	}
	cout << "0 super node：" << endl;
	for (int i = 0; i < _superNodeNumber_PIECE_0; i++){
		cout << "startit = " << *_superNodeVector_PIECE_0[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_0[i].getEndIt();
	}
	cout << endl;
}