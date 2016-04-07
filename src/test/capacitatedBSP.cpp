#include"capacitatedBSP.h"



CapacitatedBSP::CapacitatedBSP(int num) :TspBase(num){
	// used for CapacitatedBSP:
	_startFromWhichPiece = PIECE_0;
	_startStationCapacitatedBSP = -1;
	_superNodeNumber = 0;
	_sumCapacitatedBSP = 0;
	_superNodeNumber_PIECE_P = 0;
	_superNodeNumber_PIECE_N = 0;
	_superNodeNumber_PIECE_0 = 0;
	_zeroSuperNodeNumberInFront = 0;
}

CapacitatedBSP::~CapacitatedBSP(){

}

int CapacitatedBSP::getStartStationCapacitatedBSP(){
	return _startStationCapacitatedBSP;

}


void CapacitatedBSP::getSuperNodePieces(int number){
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
	//while (*it != startstation || flag){
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

void CapacitatedBSP::initMinCostAmongSuperNode(){
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

void CapacitatedBSP::calculateMinCostOfTwoSuperNode(int positivesupernode, int negativesupernode){
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

void CapacitatedBSP::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			calculateMinCostOfTwoSuperNode(i, j);
		}
	}
}

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

void CapacitatedBSP::getPositivePathReverse(int currentnumberofzeropiece, vector<int> &path){
	vector<int>::iterator it = _superNodeVector_PIECE_P[currentnumberofzeropiece].getEndIt();
	//cout << endl << "This is a positive piece:" << _superNodeVector_PIECE_P[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_P[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((path.size() > 0 && *(path.end() - 1) != *it) || path.size() == 0){
			path.push_back(*it);

		}
		//cout << *it << "P ";
		if (it == _path.begin()){
			it = _path.end() - 1;
		}
		else{
			it--;
		}
	}
}

void CapacitatedBSP::getPositivePath(int currentnumberofzeropiece, vector<int> &path){
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

void CapacitatedBSP::getNegativePathReverse(int currentnumberofzeropiece, vector<int> &path){
	vector<int>::iterator it = _superNodeVector_PIECE_N[currentnumberofzeropiece].getEndIt();
	//cout << endl << "This is a negative piece:" << _superNodeVector_PIECE_N[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_N[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((path.size() > 0 && *(path.end() - 1) != *it) || path.size() == 0){
			path.push_back(*it);

		}
		//cout << *it << "N ";
		if (it == _path.begin()){
			it = _path.end() - 1;
		}
		else{
			it--;
		}
	}
}

void CapacitatedBSP::getNegativePath(int currentnumberofzeropiece, vector<int> &path){
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

void CapacitatedBSP::getZeroPathReverse(int currentnumberofzeropiece, vector<int> &path){
	vector<int>::iterator it = _superNodeVector_PIECE_0[currentnumberofzeropiece].getEndIt();
	//cout << endl << "This is a zero piece:" << _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode() << "  ";
	for (int i = 0; i < _superNodeVector_PIECE_0[currentnumberofzeropiece].getNodeNumberInSuperNode(); i++){
		if ((path.size() > 0 && *(path.end() - 1) != *it) || path.size() == 0){
			path.push_back(*it);

		}
		//cout << *it << "& ";
		if (it == _path.begin()){
			it = _path.end() - 1;
		}
		else{
			it--;
		}
	}
}

void CapacitatedBSP::getZeroPath(int currentnumberofzeropiece, vector<int> &path){
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

// used for opposite direction:
void CapacitatedBSP::getZeroPathInBehind(vector<int> &path){
	//cout << "Front zero pieces:" << _zeroSuperNodeNumberInFront;
	int zeronum = _superNodeVector_PIECE_0.size();
	for (int i = zeronum - 1; i > zeronum - _zeroSuperNodeNumberInEnd; i--){
		getZeroPathReverse(i, path);
	}
	//cout << endl<<endl;
}

void CapacitatedBSP::getZeroPathInFront(vector<int> &path){
	//cout << "Front zero pieces:" << _zeroSuperNodeNumberInFront;
	for (int i = 0; i < _zeroSuperNodeNumberInFront; i++){
		getZeroPath(i, path);
	}
	//cout << endl<<endl;
}


void CapacitatedBSP::getPathBeginPositiveReverse(){
	vector<int> temppath;
	int currentnumberofzeropiece = _superNodeVector_PIECE_0.size() - 1;
	if (_zeroSuperNodeNumberInEnd != 0){
		getZeroPathInBehind(temppath);
		currentnumberofzeropiece -= _zeroSuperNodeNumberInEnd;
	}
	if (_superNodeNumber >= 2){
		for (int positivesupernode = _superNodeNumber / 2 - 1; positivesupernode >= 0; positivesupernode--){
			int negativesupernode = _superNodeVector_PIECE_P[positivesupernode].getMatchingNumber();
			vector<int>::iterator positiveit = _superNodeVector_PIECE_P[positivesupernode].getEndIt();
			//cout << "This is a positive piece:";
			//cout << _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode() << " ";
			for (int i = 0; i < _superNodeVector_PIECE_P[positivesupernode].getNodeNumberInSuperNode(); i++){

				if ((temppath.size() > 0 && *(temppath.end() - 1) != *positiveit) || temppath.size() == 0){
					temppath.push_back(*positiveit);
				}
				//cout << *positiveit << "P ";

				if (positiveit == _minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt){
					//cout << "This is a negative piece:";
					//	cout << "²âÊÔµã5" << endl;
					getNegativePathReverse(negativesupernode, temppath);

					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, temppath);
							--currentnumberofzeropiece;
						}
					}

					if ((temppath.size() > 0 && *(temppath.end() - 1) != *positiveit) || temppath.size() == 0){
						temppath.push_back(*positiveit);
					}
					//		cout << *positiveit << "$$$$ ";
				}// if negative

				int numberofzeropiece = 0;
				if (positiveit == _superNodeVector_PIECE_P[positivesupernode].getStartIt() &&
					(numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceFront()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						getZeroPathReverse(currentnumberofzeropiece, temppath);
						--currentnumberofzeropiece;
					}
				}

				if (positiveit == _path.begin()){
					positiveit = _path.end() - 1;
				}
				else{
					--positiveit;
				}

			}//for positive
			//cout << endl;
		}// for positive and negative
	}// if
	//cout << endl;

	int tempsum = 0;
	if ((tempsum = getFinalSum(temppath)) < _sumCapacitatedBSP){

		_sumCapacitatedBSP = tempsum;
		_finalPath.clear();
		for (vector<int>::iterator it = temppath.begin(); it < temppath.end(); ++it){
			_finalPath.push_back(*it);
		}
	}

	PRINTFFinalPath

}


void CapacitatedBSP::getPathBeginPositive(){
	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront(_finalPath);
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
				if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *positiveit) || _finalPath.size() == 0){
					_finalPath.push_back(*positiveit);
				}
				//cout << *positiveit << "P ";

				if (positiveit == _minCostAmongSuperNode[positivesupernode][negativesupernode].firstNodeIt){
					//cout << "This is a negative piece:";
					//	cout << "²âÊÔµã5" << endl;
					getNegativePath(negativesupernode, _finalPath);

					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, _finalPath);
							++currentnumberofzeropiece;
						}
					}

					if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *positiveit) || _finalPath.size() == 0){
						_finalPath.push_back(*positiveit);
					}
					//		cout << *positiveit << "$$$$ ";
				}// if negative

				int numberofzeropiece = 0;
				if (positiveit == _superNodeVector_PIECE_P[positivesupernode].getEndIt() &&
					(numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceBehind()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						getZeroPath(currentnumberofzeropiece, _finalPath);
						++currentnumberofzeropiece;
					}
				}
			}//for positive
			//cout << endl;
		}// for positive and negative
	}// if
	//cout << endl;

	PRINTFFinalPath

}


void CapacitatedBSP::getPathBeginNegativeReverse(){
	vector<int> temppath;
	int currentnumberofzeropiece = _superNodeVector_PIECE_0.size() - 1;
	if (_zeroSuperNodeNumberInEnd != 0){
		getZeroPathInBehind(temppath);
		currentnumberofzeropiece -= _zeroSuperNodeNumberInEnd;
	}
	if (_superNodeNumber >= 2){
		for (int negativesupernode = _superNodeNumber / 2 - 1; negativesupernode >= 0; negativesupernode--){
			int positivesupernode = _superNodeVector_PIECE_N[negativesupernode].getMatchingNumber();
			vector<int>::iterator negativeit = _superNodeVector_PIECE_N[negativesupernode].getEndIt();
			//cout << "This is a negative piece:";
			//cout << _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode() << " ";
			for (int i = 0; i < _superNodeVector_PIECE_N[negativesupernode].getNodeNumberInSuperNode(); i++){
				if ((temppath.size() > 0 && *(temppath.end() - 1) != *negativeit) || temppath.size() == 0){
					temppath.push_back(*negativeit);
				}
				//	cout << *negativeit << "N ";

				if (negativeit == _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt){
					//		cout << "This is a positive piece:";
					getPositivePathReverse(positivesupernode, temppath);

					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceFront()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPathReverse(currentnumberofzeropiece, temppath);
							--currentnumberofzeropiece;
						}
					}

					if ((temppath.size() > 0 && *(temppath.end() - 1) != *negativeit) || temppath.size() == 0){
						temppath.push_back(*negativeit);
					}
					//		cout << *negativeit << "$$$$ ";
				}// if negative

				int numberofzeropiece = 0;
				if (negativeit == _superNodeVector_PIECE_N[negativesupernode].getStartIt() &&
					(numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceFront()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						getZeroPathReverse(currentnumberofzeropiece, temppath);
						--currentnumberofzeropiece;
					}
				}

				if (negativeit == _path.begin()){
					negativeit = _path.end() - 1;
				}
				else{
					--negativeit;
				}
			}//for positive
			//	cout << endl;
		}// for positive and negative
	}// if
	//	cout << endl;

	int tempsum = 0;
	if ((tempsum = getFinalSum(temppath)) < _sumCapacitatedBSP){

		_sumCapacitatedBSP = tempsum;
		_finalPath.clear();
		for (vector<int>::iterator it = temppath.begin(); it < temppath.end(); ++it){
			_finalPath.push_back(*it);
		}
	}

	PRINTFFinalPath

}

void CapacitatedBSP::getPathBeginNegative(){
	int currentnumberofzeropiece = 0;
	if (_zeroSuperNodeNumberInFront != 0){
		getZeroPathInFront(_finalPath);
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
				if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *negativeit) || _finalPath.size() == 0){
					_finalPath.push_back(*negativeit);
				}
				//	cout << *negativeit << "N ";

				if (negativeit == _minCostAmongSuperNode[positivesupernode][negativesupernode].secondNodeIt){
					//		cout << "This is a positive piece:";
					getPositivePath(positivesupernode, _finalPath);

					int numberofzeropiece = 0;
					if ((numberofzeropiece = _superNodeVector_PIECE_P[positivesupernode].getNumberOfZeroPieceBehind()) != 0){
						for (int i = 0; i < numberofzeropiece; i++){
							getZeroPath(currentnumberofzeropiece, _finalPath);
							++currentnumberofzeropiece;
						}
					}

					if ((_finalPath.size() > 0 && *(_finalPath.end() - 1) != *negativeit) || _finalPath.size() == 0){
						_finalPath.push_back(*negativeit);
					}
					//		cout << *negativeit << "$$$$ ";
				}// if negative

				int numberofzeropiece = 0;
				if (negativeit == _superNodeVector_PIECE_N[negativesupernode].getEndIt() &&
					(numberofzeropiece = _superNodeVector_PIECE_N[negativesupernode].getNumberOfZeroPieceBehind()) != 0){
					for (int i = 0; i < numberofzeropiece; i++){
						getZeroPath(currentnumberofzeropiece, _finalPath);
						++currentnumberofzeropiece;
					}
				}
			}//for positive
			//	cout << endl;
		}// for positive and negative
	}// if
	//	cout << endl;

	PRINTFFinalPath

}

void CapacitatedBSP::getPath(){
	if (_startFromWhichPiece == PIECE_P){
		cout << "_startFromWhichPiece:PIECE_P" << endl;
		getPathBeginPositive();
		getPathBeginNegativeReverse();
		cout << endl;
	}
	else{
		cout << "_startFromWhichPiece:PIECE_N" << endl;
		getPathBeginNegative();
		getPathBeginPositiveReverse();
		cout << endl;;
	}
}
int CapacitatedBSP::getFinalSum(vector<int> &path){
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


// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void CapacitatedBSP::getTspTour(const std::string &alg_name) {
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

	PRINTFTSPtour
}

void CapacitatedBSP::run(){

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

	sum = clock();
	start = clock();
	getSuperNodePieces(0);
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
	cout << "\nCapacitated BSP time:" << totaltime2 << "ms! Sum cost:" << getFinalSum(_finalPath) << endl;
	cout << "length:" << _finalPath.size() << endl;

}

void CapacitatedBSP::printFinalPath(){

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

void CapacitatedBSP::printSuperNodeInformation(){

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

