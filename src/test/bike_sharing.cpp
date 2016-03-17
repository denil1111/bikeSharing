
#include"bike_sharing.h"

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

BikeSharing::BikeSharing(int num){
	_stationNum = num;
	_startStation = -1;
	g = new FullGraph(num);
	cost = new DoubleEdgeMap(*g);
	pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
}

// 这个还不支持，不能用，报错，还需要改
BikeSharing::BikeSharing(){
	_stationNum = 0;
	g = new FullGraph(0);
	cost = new DoubleEdgeMap(*g);
}

// 随机生成每个站点的需求量：（ -STATION_CAPACITY，STATION_CAPACITY ）
void BikeSharing::getRandomDemand(){
	//TspCheck tspCheck;
	int temp;
	cout << "Station Demand:" << endl;
	srand((unsigned)time(NULL));
	for (int i = 0; i < _stationNum; i++){
		temp = rand() % (STATION_CAPACITY * 2 + 1) - STATION_CAPACITY;
		_stationDemand.push_back(temp);
		printf("%3d ", temp);
		if ((i + 1) % 20 == 0){
			printf("\n");
		}
	}
	cout << endl << endl;
	//tspCheck.checkDemand(_stationDemand);
}

// 
void BikeSharing::getRandomPoints(){
	double a, b;
	srand((unsigned)time(NULL));
	cout << "Point坐标：" << endl;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		// NodeMap，是一个map，其中Node是key，值是Point点。
		a = rand() % 100;
		b = rand() % 100;
		(*pos)[u] = dim2::Point<double>(a, b);
		cout << "(" << a << ", " << b << ") ";
	}
	cout << endl << endl << endl;
}

void BikeSharing::getCost(){

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

void BikeSharing::getRandomCost(){
	int i = 0, j = 0;
	cout << "获取Cost:" << endl;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, i++) {
		for (FullGraph::NodeIt v = u; v != INVALID; ++v, j++) {
			if (u == v) {
				//cout << "cost[" << i << "][" << j << "] = " << 0 << " ";
				//cout << 0 << " ";
				continue;
			}
			(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = ((*pos)[u] - (*pos)[v]).normSquare();
			//cout << (*cost)[(*g).edge(u, v)] << " ";
		}
		//cout << endl;
		j = 0;
	}
	cout << endl;
}


// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void BikeSharing::getRandomTspSequence(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	//TspCheck tspCheck;
	TSP alg(*g, *cost);

	// 因为是完全图，要设置每一条边的cost值,这个cost也是一个map，其中g的边为key。


	_sum = alg.run();
	cout << "sum = " << _sum << endl;

	//tspCheck.check(alg.run() > 0, alg_name + ": Wrong total cost");

	std::vector<Node> vec;
	alg.tourNodes(std::back_inserter(vec));
	cout << "TSP通路：" << endl;
	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		FullGraph::Node node = *it;
		_path.push_back((*g).index(node));
		cout << " " << (*g).index(node);
	}
	cout << endl;

	// 下面都是check
	//tspCheck.check(tspCheck.checkTour(*g, vec), alg_name + ": Wrong node sequence");
	//tspCheck.check(tspCheck.checkCost(*g, vec, *cost, alg.tourCost()), alg_name + ": Wrong tour cost");

	SimplePath<FullGraph> path;
	alg.tour(path);
	//tspCheck.check(tspCheck.checkTourPath(*g, path), alg_name + ": Wrong tour");
	//tspCheck.check(tspCheck.checkCost(*g, path, *cost, alg.tourCost()),alg_name + ": Wrong tour cost");

}

// Get a TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void BikeSharing::getTspSequence(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	//TspCheck tspCheck;
	TSP alg(*g, *cost);

	_sum = alg.run();
	cout << "sum = " << _sum << endl;

	//tspCheck.check(alg.run() > 0, alg_name + ": Wrong total cost");

	std::vector<Node> vec;
	alg.tourNodes(std::back_inserter(vec));
	cout << "TSP通路：" << endl;
	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		FullGraph::Node node = *it;
		_path.push_back((*g).index(node));
		cout << " " << (*g).index(node);
	}
	cout << endl;

	// 下面都是check
	//tspCheck.check(tspCheck.checkTour(*g, vec), alg_name + ": Wrong node sequence");
	//tspCheck.check(tspCheck.checkCost(*g, vec, *cost, alg.tourCost()), alg_name + ": Wrong tour cost");

	SimplePath<FullGraph> path;
	alg.tour(path);
	//tspCheck.check(tspCheck.checkTourPath(*g, path), alg_name + ": Wrong tour");
	//tspCheck.check(tspCheck.checkCost(*g, path, *cost, alg.tourCost()),alg_name + ": Wrong tour cost");

}

bool BikeSharing::isExistNotVisitedPositiveStation(){

	return true;
}

bool BikeSharing::isAPositiveStation(int number){
	return _stationDemand[number] >= 0;
}

// 获取起始出发点：
int BikeSharing::getStartStation(){

	/*
	// 以下是测试代码
	int b[7] = { 3, 2, 5, 4, 0, 1, 6 };
	for (int i = 0; i < 7; i++){
	_path.push_back(b[i]);
	}
	int a[7] = { -1, 2, -4, -3, 2, 5, -1 };
	for (int i = 0; i < 7; i++){
	_stationDemand.push_back(a[i]);
	}
	*/
	_positiveStationVisiteFlag.clear();

	vector<int>::const_iterator it = _path.begin();
	while (isExistNotVisitedPositiveStation()){
		// 正站点访问标记,如果访问过，就放进这个vector中。如果下次又轮到这个点，说明无解
		vector<int>::iterator result = find(_positiveStationVisiteFlag.begin(), _positiveStationVisiteFlag.end(), *it);
		if (result != _positiveStationVisiteFlag.end()){
			cout << "无解，站点标号为：" << *it << endl;
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
				cout << "找到了起始点：" << _startStation << endl;
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
		}// if 一次寻找的开始
		else {
			++it;
		}
	}// while
	return -1;
}

void BikeSharing::printResult(){
	int tempSum = 0;
	int i = 0;
	vector<int>::iterator itStartStation = find(_path.begin(), _path.end(), _startStation);
	vector<int>::iterator it = itStartStation;
	cout << endl << "正确访问路径：" << endl;
	for (it; it < _path.end(); it++){
		cout << *it << "(" << _stationDemand[*it] << ") ";
	}
	for (it = _path.begin(); it < itStartStation; ++it){
		cout << *it << "(" << _stationDemand[*it] << ") ";
	}
	cout << endl << endl;
}

void BikeSharing::runRandom(){

	GRAPH_TYPEDEFS(FullGraph);

	DWORD start_time = GetTickCount();
	DWORD end_time;

	getRandomPoints();
	getRandomCost();
	getRandomDemand();

	end_time = GetTickCount();
	cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;//输出运行时间

	getRandomTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");

	end_time = GetTickCount();
	cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;//输出运行时间

	getStartStation();
	printResult();

	end_time = GetTickCount();
	cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;//输出运行时间

}

void BikeSharing::run(){

	DWORD start_time = GetTickCount();
	DWORD end_time;

	getRandomPoints();
	getCost();
	getRandomDemand();

	end_time = GetTickCount();
	cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;//输出运行时间

	getTspSequence< ChristofidesTsp<DoubleEdgeMap > >("Christofides");

	end_time = GetTickCount();
	cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;//输出运行时间

	getStartStation();
	printResult();

	end_time = GetTickCount();
	cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;//输出运行时间

}

