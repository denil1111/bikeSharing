#include"uncapacitatedBSP.h"


UncapacitatedBSP::UncapacitatedBSP(int num) :TspBase(num){
	// used for UncapacitatedBSP:
	_startStationUncapacitatedBSP = -1;
}

UncapacitatedBSP::~UncapacitatedBSP(){

}

bool UncapacitatedBSP::isExistNotVisitedPositiveStation(){
	return true;
}

bool UncapacitatedBSP::isAPositiveStation(int number){
	return _stationDemand[number] >= 0;
}

int UncapacitatedBSP::getStartStation(){
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

// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void UncapacitatedBSP::getTspTour(const std::string &alg_name) {
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

void UncapacitatedBSP::run(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
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

}
