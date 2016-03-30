/*
*
*
*
*/

#include"uncapacitatedBSP.h"


typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

UncapacitatedBSP::UncapacitatedBSP(int num){
	_stationNum = num;
	_startStation = -1;
	g = new FullGraph(num);
	cost = new DoubleEdgeMap(*g);
	pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
}

// 
void UncapacitatedBSP::getRandomPoints(){
	double a, b;
	srand((unsigned)time(NULL));

	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		a = rand() % 100;
		b = rand() % 100;
		(*pos)[u] = dim2::Point<double>(a, b);
	}

	PRINTFRandomPoints
}

void UncapacitatedBSP::printRandomPoints(){
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

// random:（ -STATION_CAPACITY，STATION_CAPACITY ）
void UncapacitatedBSP::getRandomDemand(){
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

void UncapacitatedBSP::printRandomDemand(){
	cout << "Station Demand:" << endl;
	for (int i = 0; i < _stationNum; i++){
		printf("%3d ", _stationDemand[i]);
		if ((i + 1) % 20 == 0){
			cout << endl;
		}
	}
	cout << endl << endl;
}

void UncapacitatedBSP::getCost(){
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		for (FullGraph::NodeIt v = u; v != INVALID; ++v) {
			if (u != v) {
				(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = ((*pos)[u] - (*pos)[v]).normSquare();
			}
		}
	}
	PRINTFCost
}

void UncapacitatedBSP::printCost(){
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

/*
void UncapacitatedBSP::getCost(){

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
void UncapacitatedBSP::getTspTour(const std::string &alg_name) {
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

void UncapacitatedBSP::printTSPtour(){
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
		// 正站点访问标记,如果访问过，就放进这个vector中。如果下次又轮到这个点，说明无解
		vector<int>::iterator result = find(_positiveStationVisiteFlag.begin(), _positiveStationVisiteFlag.end(), *it);
		if (result != _positiveStationVisiteFlag.end()){
			cout << "No solution!!!!!" <<endl;
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

void UncapacitatedBSP::printResult(){
	int tempSum = 0;
	int i = 0;
	vector<int>::iterator itStartStation = find(_path.begin(), _path.end(), _startStation);
	vector<int>::iterator it = itStartStation;
	cout << endl << "Final Result:" << endl;
	for (it; it < _path.end(); it++){
		cout << *it << "(" << _stationDemand[*it] << ") ";
	}
	for (it = _path.begin(); it < itStartStation; ++it){
		cout << *it << "(" << _stationDemand[*it] << ") ";
	}
	cout << endl << endl;
}

void UncapacitatedBSP::runRandom(){

	clock_t start, finish, sum;
	double totaltime;
	sum = clock();

	cout << "UncapacitatedBSP:" << endl << endl;
	//cout << "station num:" << STATION_NUM << endl;
	cout << "vehicle capacity:" << VEHICLE_CAPACITY << endl;
	cout << "station capacity:" << STATION_CAPACITY << endl;

	start = clock();
	getRandomPoints();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetRandomPoints:" << totaltime << "ms！" << endl;

	getCost();
	start = finish;
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetCost:" << totaltime << "ms！" << endl;

	getRandomDemand();
	start = finish;
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetRandomDemand:" << totaltime << "ms！" << endl;


	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	start = finish;
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetTspTour:" << totaltime << "ms！" << endl;

	getStartStation();
	start = finish;
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetStartStation:" << totaltime << "ms！" << endl;

	totaltime = (double)(finish - sum) / CLOCKS_PER_SEC * 1000;
	cout << "\nUncapacitated BSP time:" << totaltime << "ms！ Sum cost:" << _sum << endl << endl;

}

