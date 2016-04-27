#include"ktimescapacitatedBSP.h"

KTimesCapacitatedBSP::KTimesCapacitatedBSP(TspBase &tspbase):BspBase(tspbase){
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

KTimesCapacitatedBSP::~KTimesCapacitatedBSP(){

}

void KTimesCapacitatedBSP::getSuperNodePieces(int number){
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

void KTimesCapacitatedBSP::calculateMinCostAmongSuperNode(){
	initMinCostAmongSuperNode();
	for (int i = 0; i < _superNodeNumber / 2; i++){
		for (int j = 0; j < _superNodeNumber / 2; j++){
			calculateMinCostOfTwoSuperNode(i, j);
		}
	}
}

void KTimesCapacitatedBSP::run(){
    _tspBase.data();
	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	cout << endl << "ktimescapacitatedBSP:" << endl;

	for (int i = 0; i < _tspBase.VEHICLE_CAPACITY / 2; i++){
		initSuperNode();
		getSuperNodePieces(i);
		calculateMinCostAmongSuperNode();
		machingSuperNode();
		getPath();
		
		//cout << "Get a path " << i << endl << endl;
	}
	
	deleteRepeatStationPoint(_minCostPath);
	getStartStation(_minCostPath);
	revertPath(_minCostPath);
	mapPath();
	
	PRINTFFinalPath

	cout << "ktimescapacitatedBSP sum cost:" << _minSum << endl;
	//cout << "Mininum sum cost:" << _minSum << endl;
	//cout << "StartStation:" << _startStationCapacitatedBSP << endl;
	//cout << "length:" << _minCostPath.size() << endl;
	//cout << "Path set size:" <<_pathSet.size() << endl;

}