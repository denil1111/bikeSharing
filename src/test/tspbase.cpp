#include"tspbase.h"

TspBase::TspBase(int num){
	_tspSum = 0;
	_stationNum = num;
	g = new FullGraph(num);
	cost = new DoubleEdgeMap(*g);
	pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
}

TspBase::~TspBase(){
	delete g;
	delete cost;
	delete pos;
}

// random:�� -STATION_CAPACITY��STATION_CAPACITY ��
void TspBase::getRandomDemand(){
	int temp;
	int sum = 0;

	srand((unsigned)time(NULL));
	for (int i = 0; i < _stationNum; i++){
		while ((temp = rand() % (STATION_CAPACITY * 2 + 1) - STATION_CAPACITY) == 0){
			;
		}
		_stationDemand.push_back(temp);
		sum += temp;
	}
	sum -= temp;
	srand((unsigned)time(NULL));
	int pointnum;
	if (sum > STATION_CAPACITY){
		_stationDemand[_stationNum - 1] = -(sum % STATION_CAPACITY);
		while (sum >= STATION_CAPACITY){
			pointnum = rand() % _stationNum;
			_stationDemand.insert(_stationDemand.begin() + pointnum, -STATION_CAPACITY);
			sum -= STATION_CAPACITY;
			++_stationNum;
		}
		delete g;
		delete cost;
		delete pos;
		g = new FullGraph(_stationNum);
		cost = new DoubleEdgeMap(*g);
		pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
	}
	else if (sum < -STATION_CAPACITY){
		_stationDemand[_stationNum - 1] = -(sum % STATION_CAPACITY);
		while (sum <= -STATION_CAPACITY){
			pointnum = rand() % _stationNum;
			_stationDemand.insert(_stationDemand.begin() + pointnum, STATION_CAPACITY);
			sum += STATION_CAPACITY;
			++_stationNum;
		}
		delete g;
		delete cost;
		delete pos;
		g = new FullGraph(_stationNum);
		cost = new DoubleEdgeMap(*g);
		pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
	}
	else{
		_stationDemand[_stationNum - 1] = -sum;
	}

	CHECKRandomDemand
	PRINTFDemand

}

void TspBase::checkRandomDemand(){
	int sum = 0;
	for (int i = 0; i < _stationNum; i++){
		sum += _stationDemand[i];
	}
	if (sum != 0){
		cout << "Sum of demand wrong!!!" << endl;
	}
	else{
		cout << "Sum of demand equal zero." << endl;
	}
}

void TspBase::getRandomPoints(){
	srand((unsigned)time(NULL));
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		point poss;
		poss.a = rand() % POINT_RANGE;
		poss.b = rand() % POINT_RANGE;
		(*pos)[u] = dim2::Point<int>(poss.a, poss.b);
		_point.push_back(poss);
	}
}

void TspBase::getPoints(){
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		_point[i].a = rand() % POINT_RANGE;
		_point[i].b = rand() % POINT_RANGE;
		(*pos)[u] = dim2::Point<int>(_point[i].a, _point[i].b);
	}
	PRINTFPoints
}

void TspBase::getDemand(){

}

void TspBase::getCost(){
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

void TspBase::getRandomCost(){
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		int j = 0;
		vector<int> costrow;
		for (FullGraph::NodeIt v = u; v != INVALID; ++v, ++j) {
			if (u != v) {
				(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = (int)sqrt(((*pos)[u] - (*pos)[v]).normSquare());
				costrow.push_back((int)sqrt(((*pos)[u] - (*pos)[v]).normSquare()));
			}
			else{
				costrow.push_back(0);
			}
		}
		_cost.push_back(costrow);
	}
	// PRINTFCost
}

// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void TspBase::getTspTour(const std::string &alg_name) {
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

void TspBase::randomData(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	cout << "vehicle capacity:" << VEHICLE_CAPACITY << endl;
	cout << "station capacity:" << STATION_CAPACITY << endl;

	start = clock();
	getRandomDemand();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetRandomDemand:" << totaltime << "ms!" << endl;

	start = clock();
	getRandomPoints();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetRandomPoints:" << totaltime << "ms!" << endl;

	start = clock();
	getRandomCost();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	cout << "\ngetCost:" << totaltime << "ms!" << endl;

}

void TspBase::printPoints(){
	cout << "Point:" << endl;
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u){
		cout << "(" << (*pos)[u].x << ", " << (*pos)[u].y << ") ";
		if ((i++ + 1) % 10 == 0){
			cout << endl;
		}
	}
	cout << endl << endl;
}

void TspBase::printDemand(){
	cout << "Station Demand:" << endl;
	for (int i = 0; i < _stationNum; i++){
		printf("%3d ", _stationDemand[i]);
		if ((i + 1) % 20 == 0){
			cout << endl;
		}
	}
	cout << endl << endl;
}


void TspBase::printCost(){
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

void TspBase::printTSPtour(){
	int i = 0;
	cout << "TSP tour:" << endl;
	for (vector<int>::iterator it = _path.begin(); it < _path.end(); ++it){
		cout << *it << " ";
		if ((i++ + 1) % 30 == 0){
			cout << endl;
		}
	}
	cout << endl;
	cout << "TSP tour demand sum: " << _tspSum << endl << endl;
}