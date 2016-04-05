#include"ktimescapacitatedBSP.h"



KTimesCapacitatedBSP::KTimesCapacitatedBSP(int num) :TspBase(num){
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

KTimesCapacitatedBSP::~KTimesCapacitatedBSP(){

}

// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void KTimesCapacitatedBSP::getTspTour(const std::string &alg_name) {
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

int KTimesCapacitatedBSP::getStartStationCapacitatedBSP(){
	return _startStationCapacitatedBSP;

}

void KTimesCapacitatedBSP::getSuperNodePieces(int number){
	int lastsupernodeflag = PIECE_NULL;

	vector<int>::iterator it = _path.begin();
	for (int i = 0; i < number; i++){
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
	_startStationCapacitatedBSP = *it;

	//cout << "Start station id:" << _startStationCapacitatedBSP << " "<<_stationDemand[*it] <<endl << endl;
	int surplusdemand = 0;

	int flag = true;
	int stopflag = 0;
	//while (*it != _startStationCapacitatedBSP || flag){
	while (stopflag < _stationNum){
		flag = false;

		SuperNode newpiece(Q / 2);
		surplusdemand = newpiece.getASuperNode(_path, _stationDemand, _startStationCapacitatedBSP, it, surplusdemand, stopflag, _stationNum);
		if (newpiece.getPieceTypeFlag() == PIECE_P){
			_superNodeNumber_PIECE_P++;
			++_superNodeNumber;
			_superNodeVector_PIECE_P.push_back(newpiece);
			lastsupernodeflag = PIECE_P;

			if (_startFromWhichPiece == PIECE_0){
				_startFromWhichPiece = PIECE_P;
				cout << "startpoint demond" << *newpiece.getStartIt() << " " << _stationDemand[*newpiece.getStartIt()] << endl;
			}

		}
		else if (newpiece.getPieceTypeFlag() == PIECE_N){
			_superNodeNumber_PIECE_N++;
			++_superNodeNumber;
			_superNodeVector_PIECE_N.push_back(newpiece);
			lastsupernodeflag = PIECE_N;

			if (_startFromWhichPiece == PIECE_0){
				_startFromWhichPiece = PIECE_N;
				cout << "startpoint demond" << *newpiece.getStartIt() << " " << _stationDemand[*newpiece.getStartIt()] << endl;
			}

		}
		else{
			_superNodeVector_PIECE_0.push_back(newpiece);
			_superNodeNumber_PIECE_0++;
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
}

void KTimesCapacitatedBSP::initMinCostAmongSuperNode(){
	_minCostAmongSuperNode.clear();
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

void KTimesCapacitatedBSP::calculateMinCostOfTwoSuperNode(int positivesupernode, int negativesupernode){
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

void KTimesCapacitatedBSP::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			calculateMinCostOfTwoSuperNode(i, j);
		}
	}
}

string KTimesCapacitatedBSP::getLGF(){
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

void KTimesCapacitatedBSP::machingSuperNode(){
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

void KTimesCapacitatedBSP::getPositivePath(int currentnumberofzeropiece, vector<int> &path){
	vector<int>::iterator it = _superNodeVector_PIECE_P[currentnumberofzeropiece].getStartIt();
	//cout << endl << "This is a positive piece:" << _superNodeVector_PIECE_P[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_P[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((path.size() > 0 && *(path.end() - 1) != *it) || path.size() == 0){
			path.push_back(*it);

		}
		//cout << *it << "P ";
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
}

void KTimesCapacitatedBSP::getNegativePath(int currentnumberofzeropiece, vector<int> &path){
	vector<int>::iterator it = _superNodeVector_PIECE_N[currentnumberofzeropiece].getStartIt();
	//cout << endl << "This is a negative piece:" << _superNodeVector_PIECE_N[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_N[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((path.size() > 0 && *(path.end() - 1) != *it) || path.size() == 0){
			path.push_back(*it);

		}
		//cout << *it << "N ";
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
}

void KTimesCapacitatedBSP::getZeroPath(int currentnumberofzeropiece, vector<int> &path){
	vector<int>::iterator it = _superNodeVector_PIECE_0[currentnumberofzeropiece].getStartIt();
	//cout << endl << "This is a zero piece:" << _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((path.size() > 0 && *(path.end() - 1) != *it) || path.size() == 0){
			path.push_back(*it);

		}
		//cout << *it << "& ";
		if (++it == _path.end()){
			it = _path.begin();
		}
	}
}

void KTimesCapacitatedBSP::getZeroPathInFront(vector<int> &path){
	//cout << "Front zero pieces:" << _zeroSuperNodeNumberInFront;
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPath(i, path);
	}
	//cout << endl<<endl;
}


void KTimesCapacitatedBSP::getPathBeginPositive(){
	vector<int> temppath;
	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront(temppath);
		currentnumberofzeropiece += _zeroSuperNodeNumberInFront;
	}
	if (_superNodeNumber >= 2){
		for (int positivesupernode = 0; positivesupernode < _superNodeNumber / 2; positivesupernode++){
			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();
			vector<int>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode].getStartIt();
			//cout << "This is a positive piece:";
			//cout << _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode() << " ";
			for (int i = 0; i < _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode(); i++, ++positiveit){
				if (positiveit == _path.end()){
					positiveit = _path.begin();
				}
				if ((temppath.size() > 0 && *(temppath.end() - 1) != *positiveit) || temppath.size() == 0){
					temppath.push_back(*positiveit);
				}
				//cout << *positiveit << "P ";
				
				if (positiveit == _minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt){
					//cout << "This is a negative piece:";
				//	cout << "²âÊÔµã5" << endl;
					getNegativePath(negativesupernode, temppath);

					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, temppath);
							++currentnumberofzeropiece;
						}
					}

					if ((temppath.size() > 0 && *(temppath.end() - 1) != *positiveit) || temppath.size() == 0){
						temppath.push_back(*positiveit);
					}
			//		cout << *positiveit << "$$$$ ";
				}// if negative

				int numberofzeropiece = 0;
				if (positiveit == _superNodeVector_PIECE_P[positivesupernode].getEndIt() && 
					(numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceBehind()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						getZeroPath(currentnumberofzeropiece, temppath);
						++currentnumberofzeropiece;
					}
				}
			}//for positive
			//cout << endl;
		}// for positive and negative
	}// if
	//cout << endl;
	_pathSet.push_back(temppath);

	int tempsum = 0;
	if ((tempsum = getFinalSum(temppath)) < _minSum){
		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<int>::iterator it = temppath.begin(); it < temppath.end(); ++it){
			_minCostPath.push_back(*it);
		}
	}

	PRINTFFinalPath

}

void KTimesCapacitatedBSP::getPathBeginNegative(){
	vector<int> temppath;
	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront(temppath);
		currentnumberofzeropiece += _zeroSuperNodeNumberInFront;
	}
	if (_superNodeNumber >= 2){
		for (int negativesupernode = 0; negativesupernode < _superNodeNumber / 2; negativesupernode++){
			int positivesupernode = _superNodeVector_PIECE_N[negativesupernode].getMatchingNumber();
			vector<int>::iterator negativeit = _superNodeVector_PIECE_N[negativesupernode].getStartIt();
			//cout << "This is a negative piece:";
			//cout << _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode() << " ";
			for (int i = 0; i < _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode(); i++, ++negativeit){
				if (negativeit == _path.end()){
					negativeit = _path.begin();
				}
				if ((temppath.size() > 0 && *(temppath.end() - 1) != *negativeit) || temppath.size() == 0){
					temppath.push_back(*negativeit);
				}
			//	cout << *negativeit << "N ";

				if (negativeit == _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt){
			//		cout << "This is a positive piece:";
					getPositivePath(positivesupernode, temppath);

					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, temppath);
							++currentnumberofzeropiece;
						}
					}

					if ((temppath.size() > 0 && *(temppath.end() - 1) != *negativeit) || temppath.size() == 0){
						temppath.push_back(*negativeit);
					}
			//		cout << *negativeit << "$$$$ ";
				}// if negative

				int numberofzeropiece = 0;
				if (negativeit == _superNodeVector_PIECE_N[negativesupernode].getEndIt() &&
					(numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						getZeroPath(currentnumberofzeropiece, temppath);
						++currentnumberofzeropiece;
					}
				}
			}//for positive
		//	cout << endl;
		}// for positive and negative
	}// if
//	cout << endl;
	_pathSet.push_back(temppath);

	int tempsum = 0;
	if ((tempsum = getFinalSum(temppath)) < _minSum){
		_minSum = tempsum;
		_minCostPath.clear();
		for (vector<int>::iterator it = temppath.begin(); it < temppath.end(); ++it){
			_minCostPath.push_back(*it);
		}
	}

	PRINTFFinalPath

}

void KTimesCapacitatedBSP::getPath(){
	if (_startFromWhichPiece == PIECE_P){
		cout << "_startFromWhichPiece:PIECE_P" << endl;
		getPathBeginPositive();
	}
	else{
		cout << "_startFromWhichPiece:PIECE_N" << endl;
		getPathBeginNegative();
	}
}

int KTimesCapacitatedBSP::getFinalSum(vector<int> &path){
	FullGraph::Node unode, vnode;
	int i = 0;
	int tempsum = 0;

	vector<int>::iterator it = path.begin();
	unode = (*g)(*(it++));
	//vnode = (*g)(*negativeit);
	for (; it < path.end(); ++it){
		vnode = (*g)(*it);
		if (vnode != unode){
			tempsum += (*cost)[(*g).edge(unode, vnode)];
			///*cout << (*cost)[(*g).edge(unode, vnode)] << " ";
			//if (i++ % 20 == 0){
			//	cout << endl;
			//}*/
		}
		unode = vnode;
	}
	//cout << endl;

	return tempsum;
}

void KTimesCapacitatedBSP::initSuperNode(){
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

void KTimesCapacitatedBSP::run(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	// Capacitated BSP:
	cout << endl << "CapacitatedBSP:" << endl;

	///*sum = clock();
	//start = clock();
	//getSuperNodePieces();
	//finish = clock();
	//totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetSuperNodePieces:" << totaltime << "ms!" << endl;

	//start = clock();
	//calculateMinCostAmongSuperNode();
	//finish = clock();
	//totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ncalculateMinCostAmongSuperNode:" << totaltime << "ms!" << endl;

	//start = clock();
	//machingSuperNode();
	//finish = clock();
	//totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\nmachingSuperNode:" << totaltime << "ms!" << endl;

	//start = clock();
	//getPath();
	//finish = clock();
	//totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetPath:" << totaltime << "ms!" << endl;

	//totaltime2 = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;
	//cout << "\nCapacitated BSP time:" << totaltime2 << "ms! Sum cost:" << _minSum << endl;
	//cout << "length:" << _minCostPath.size() << endl;*/
	for (int i = 0; i < Q / 2; i++){
		initSuperNode();
		getSuperNodePieces(i);
		calculateMinCostAmongSuperNode();
		machingSuperNode();
		getPath();
		cout << "Get a path " << i << endl;
	}
	cout <<"Mininum sum cost:" << _minSum << endl;
	cout << "length:" << _minCostPath.size() << endl;
	cout << "Path set size:" << _pathSet.size() << endl;

}

void KTimesCapacitatedBSP::printTspPath(){
	cout << "TSP tour:" << _path.size() << endl;
	for (int i = 0; i < _path.size(); i++){
		cout << _path[i] << "(" << _stationDemand[_path[i]] << ") ";
	}
	cout << endl;
}

void KTimesCapacitatedBSP::printFinalPath(){

	cout << "Final Path:" << _minCostPath.size() << endl;
	for (int i = 0; i < _minCostPath.size(); i++){
		cout << _minCostPath[i] << "(" << _stationDemand[_minCostPath[i]] << ") ";
	}
	cout << endl;
}

void KTimesCapacitatedBSP::printSuperNodeInformation(){

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

