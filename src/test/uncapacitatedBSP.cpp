#include"uncapacitatedBSP.h"


UncapacitatedBSP::UncapacitatedBSP(TspBase &tspbase){
	_tspBase = tspbase;
	_startStationUncapacitatedBSP = -1;
}

UncapacitatedBSP::~UncapacitatedBSP(){

}

//// Get a Random TSP sequence
//// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
//template <typename TSP>
//void UncapacitatedBSP::getTspTour(const std::string &alg_name) {
//	GRAPH_TYPEDEFS(FullGraph);
//
//	TSP alg(*_tspBase.g, *_tspBase.cost);
//
//	_tspBase._tspSum = alg.run();
//
//	std::vector<Node> vec;
//	alg.tourNodes(std::back_inserter(vec));
//
//	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
//	{
//		FullGraph::Node node = *it;
//		_tspBase._path.push_back((*_tspBase.g).index(node));
//	}
//
//	PRINTFTSPtour
//}

void UncapacitatedBSP::getPath(){
	for (vector<int>::iterator it = _tspBase._path.begin(); it < _tspBase._path.end(); ++it){
		StationidAndDemand temp;
		temp.stationId = *it;
		temp.stationDemand = _tspBase._stationDemand[*it];
		_minCostPath.push_back(temp);
	}
}
//
//bool UncapacitatedBSP::isExistNotVisitedPositiveStation(){
//	return true;
//}
//
//bool UncapacitatedBSP::isAPositiveStation(int number){
//	return _tspBase._stationDemand[number] >= 0;
//}
//
//int UncapacitatedBSP::getStartStation(){
//	_positiveStationVisiteFlag.clear();
//	_startPoint = 0;
//	vector<int>::const_iterator it = _tspBase._path.begin();
//	while (isExistNotVisitedPositiveStation()){
//		// if a positive demond station have been visited:
//		vector<int>::iterator result = find(_positiveStationVisiteFlag.begin(), _positiveStationVisiteFlag.end(), *it);
//		if (result != _positiveStationVisiteFlag.end()){
//			cout << "No solution!!!!!" << endl;
//			break;
//		}
//		//cout << "*";
//		// find a station which demond is positive:
//		if (isAPositiveStation(*it)){
//			
//			_positiveStationVisiteFlag.push_back(*it);
//			_startStationUncapacitatedBSP = *it;
//			int tempSum = _tspBase._stationDemand[*it];
//			int tempNum = 1;
//
//			while (tempSum >= 0 && tempNum < _tspBase._stationNum){
//				++it;
//				++tempNum;
//				if (it == _tspBase._path.end()){
//					it = _tspBase._path.begin();
//				}
//				tempSum += _tspBase._stationDemand[*it];
//
//			}
//
//			if (tempNum == _tspBase._stationNum){
//				return _startStationUncapacitatedBSP;
//			}
//
//			if (tempSum < 0){
//				++it;
//				++tempNum;
//				if (it == _tspBase._path.end()){
//					it = _tspBase._path.begin();
//				}
//				while (_tspBase._stationDemand[*it] < 0){
//					tempSum += _tspBase._stationDemand[*it];
//					++it;
//					if (it == _tspBase._path.end()){
//						it = _tspBase._path.begin();
//					}
//				}
//			}
//		}// if 
//		else {
//			++it;
//		}
//		++_startPoint;
//	}// while
//	//cout << endl;
//	return -1;
//}

//void UncapacitatedBSP::revertPath(){
//	for (vector<int>::iterator it = _tspBase._path.begin(); it < _tspBase._path.end(); ++it){
//		StationidAndDemand temp;
//		temp.stationId = *it;
//		temp.stationDemand = _tspBase._stationDemand[*it];
//		_minCostPath.push_back(temp);
//	}
//
//	for (int i = 0; i < _startPoint; i++){
//		//cout << "*";
//		StationidAndDemand temp = *_minCostPath.begin();
//		_minCostPath.erase(_minCostPath.begin());
//		_minCostPath.push_back(temp);
//	}
//	//cout << endl;
//}



void UncapacitatedBSP::run(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetTspTour:" << totaltime << "ms!" << endl;

	totaltime0 = finish - sum;

	cout << "UncapacitatedBSP:" << endl;

	getPath();

	sum = clock();
	start = clock();
	//getStartStation();
	// getStartStation(_minCostPath);
	revertPath(_minCostPath);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetStartStation:" << totaltime << "ms!" << endl;
	mapPath();

	totaltime1 = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;

	PRINTFFinalPath

	cout << "Uncapacitated BSP sum cost:" << _tspBase._tspSum << endl << endl;

}
