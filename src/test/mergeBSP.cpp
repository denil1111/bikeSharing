 #include"mergeBSP.h"


MergeBSP::MergeBSP(int num):TspBase(num){

	// used for UncapacitatedBSP:
	_startStationUncapacitatedBSP = -1;

	// used for CapacitatedBSP:
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_sumCapacitatedBSP = 0;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
}

MergeBSP::~MergeBSP(){

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

			// 
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

int MergeBSP::getStartStationCapacitatedBSP(){
	return _startStationCapacitatedBSP;
}

void MergeBSP::getSuperNodePieces(){
	int lastsupernodeflag = PIECE_0;

	vector<int>::iterator it = _path.begin();
	while (_stationDemand[*it] < 0){
		++it;
	}
	_startStationCapacitatedBSP = *it;
	cout << "Start station id:" << _startStationCapacitatedBSP << endl << endl;
	int surplusdemand = 0;

	bool flag = true;
	while (*it != _startStationCapacitatedBSP || flag){
		flag = false;
		SuperNode newpiece(Q / 2);
		surplusdemand = newpiece.getASuperNode(_path, _stationDemand, _startStationCapacitatedBSP, it, surplusdemand);
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

void MergeBSP::getZeroPath(int currentnumberofzeropiece){
	vector<int>::iterator it = _superNodeVector_PIECE_0[currentnumberofzeropiece].getStartIt();
	//cout << endl << "这是一个零块:" << _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *it) || _finalPath.size() == 0){
			_finalPath.push_back(*it);

		}
		//cout << *it << "零 ";
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
}

void MergeBSP::getZeroPathInFront(){
	//cout << "最前面的零块：";
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPath(i);
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
				if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *positiveit) || _finalPath.size() == 0){
					_finalPath.push_back(*positiveit);
				}

				//cout << *positiveit << "正 ";
				if (positiveit == _minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt){
					vector<int>::iterator negativestartit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
					vector<int>::iterator negativeendit = _superNodeVector_PIECE_N[negativesupernode].getEndIt();
					vector<int>::iterator nagetiveit = _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt;
					vector<int>::iterator targetit = _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt;
					//cout << "这是一个负块:";
					if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *nagetiveit) || _finalPath.size() == 0){
						_finalPath.push_back(*nagetiveit);
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
						if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *nagetiveit) || _finalPath.size() == 0){
							_finalPath.push_back(*nagetiveit);
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
					if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *targetit) || _finalPath.size() == 0){
						_finalPath.push_back(*targetit);
					}
					//cout << *nagetiveit << "负 ";
					if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *positiveit) || _finalPath.size() == 0){
						_finalPath.push_back(*positiveit);
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
	if (currentnumberofzeropiece != _superNodeVector_PIECE_0.size()){
		cout << "最后一个零块：" << endl;
		getZeroPath(_superNodeVector_PIECE_0.size() - 1);
	}

	PRINTFFinalPath

}

int MergeBSP::getFinalSum(){
	FullGraph::Node unode, vnode;

	int i = 0;
	vector<int>::iterator it = _finalPath.begin();
	unode = (*g)(*(it++));
	//vnode = (*g)(*negativeit);
	for (; it < _finalPath.end(); ++it){
		vnode = (*g)(*it);
		if (vnode != unode){
			_sumCapacitatedBSP += (*cost)[(*g).edge(unode, vnode)];
			///*cout << (*cost)[(*g).edge(unode, vnode)] << " ";
			//if (i++ % 20 == 0){
			//	cout << endl;
			//}*/
		}
		unode = vnode;
	}
	//cout << endl;

	return _sumCapacitatedBSP;
}


// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void MergeBSP::getTspTour(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	//TspCheck tspCheck;
	TSP alg(*g, *cost);

	_tspSum = alg.run();

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

void MergeBSP::run(){

	clock_t start, finish, sum;
	double totaltime,totaltime0,totaltime1,totaltime2;
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

	// Capacitated BSP:
	cout << endl << "CapacitatedBSP:" << endl;

	sum = clock();
	start = clock();
	getSuperNodePieces();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetSuperNodePieces:" << totaltime << "ms!" << endl;

	start = clock();
	calculateMinCostAmongSuperNode();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ncalculateMinCostAmongSuperNode:" << totaltime << "ms!" << endl;

	start = clock();
	machingSuperNode();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\nmachingSuperNode:" << totaltime << "ms!" << endl;

	start = clock();
	getPath();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetPath:" << totaltime << "ms!" << endl;

	totaltime2 = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;
	cout << "\nCapacitated BSP time:" << totaltime2 << "ms! Sum cost:" << getFinalSum() << endl;
	cout << "length:" << _finalPath.size() << endl;

}

void MergeBSP::printFinalPath(){

	cout << "TSP tour:" << _path.size() << endl;
	for (int i = 0; i < _path.size(); i++){
		cout << _path[i] << "(" << _stationDemand[_path[i]] << ") ";
	}
	cout << endl;

	cout << "Final Path:" << _finalPath.size() << endl;
	for (int i = 0; i < _finalPath.size(); i++){
		cout << _finalPath[i] << "(" << _stationDemand[_finalPath[i]] << ") ";
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
		cout << "  mincostpoint = " << _superNodeVector_PIECE_N[i].getMatchingNumber() << endl;
		cout << "  matching point :" << *_minCostAmongSuperNode[_superNodeVector_PIECE_N[i].getMatchingNumber()][i].secondNodeIt << endl;
	}
	cout << "0 super node:" << endl;
	for (int i = 0; i < _superNodeNumber_PIECE_0; i++){
		cout << "startit = " << *_superNodeVector_PIECE_0[i].getStartIt() << "  endit = " << *_superNodeVector_PIECE_0[i].getEndIt();
	}
	cout << endl;
}