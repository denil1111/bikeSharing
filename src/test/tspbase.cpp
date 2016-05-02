#include"tspbase.h"

TspBase::TspBase(){

}

TspBase::TspBase(int num, int stationcapacity, int vehiclecapacity){
	STATION_CAPACITY = stationcapacity;
	VEHICLE_CAPACITY = vehiclecapacity;
	_tspSum = 0;
	_stationNum = num;
	_allStationNum = num;
	g = new FullGraph(num);
	cost = new DoubleEdgeMap(*g);
	pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
	_depot.a = 0;
	_depot.b = 0;
}

TspBase::TspBase(int num, int stationcapacity, int vehiclecapacity, double x, double y){
	STATION_CAPACITY = stationcapacity;
	VEHICLE_CAPACITY = vehiclecapacity;
	_tspSum = 0;
	_stationNum = num;
	_allStationNum = num;
	g = new FullGraph(num);
	cost = new DoubleEdgeMap(*g);
	pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
	_depot.a = x;
	_depot.b = y;
}

TspBase::~TspBase(){
	//delete g;
	//delete cost;
	//delete pos;
}


void TspBase::setDepot(double x, double y){
	_depot.a = x;
	_depot.b = y;
}

// random: -STATION_CAPACITY--STATION_CAPACITY 
void TspBase::getRandomDemand(){
	int temp;
	int sum = 0;

	srand((unsigned)time(NULL));
	for (int i = 0; i < _stationNum; i++){
		while ((temp = rand() % (STATION_CAPACITY * 2 + 1) - STATION_CAPACITY) == 0){
			;
		}
		_allStationDemand.push_back(temp);
		sum += temp;
	}
	sum -= temp;
	srand((unsigned)time(NULL));
	int pointnum;
	if (sum > STATION_CAPACITY){
		_allStationDemand[_stationNum - 1] = -(sum % STATION_CAPACITY);
		while (sum >= STATION_CAPACITY){
			pointnum = rand() % _stationNum;
			_allStationDemand.insert(_allStationDemand.begin() + pointnum, -STATION_CAPACITY);
			sum -= STATION_CAPACITY;
			++_stationNum;
			++_allStationNum;
		}
		delete g;
		delete cost;
		delete pos;
		g = new FullGraph(_stationNum);
		cost = new DoubleEdgeMap(*g);
		pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
	}
	else if (sum < -STATION_CAPACITY){
		_allStationDemand[_stationNum - 1] = -(sum % STATION_CAPACITY);
		while (sum <= -STATION_CAPACITY){
			pointnum = rand() % _stationNum;
			_allStationDemand.insert(_allStationDemand.begin() + pointnum, STATION_CAPACITY);
			sum += STATION_CAPACITY;
			++_stationNum;
			++_allStationNum;
		}
		delete g;
		delete cost;
		delete pos;
		g = new FullGraph(_stationNum);
		cost = new DoubleEdgeMap(*g);
		pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
	}
	else{
		_allStationDemand[_stationNum - 1] = -sum;
	}

	checkDemand();
	PRINTFDemand

}

// random: -STATION_CAPACITY--STATION_CAPACITY 
void TspBase::getInputDemand(){
	_allStationDemand.push_back(3);
	_allStationDemand.push_back(5);
	_allStationDemand.push_back(-8);
	_allStationDemand.push_back(5);
	_allStationDemand.push_back(-5);
	_allStationDemand.push_back(8);
	_allStationDemand.push_back(-7);
	_allStationDemand.push_back(-1);
	_allStationDemand.push_back(23);
	_allStationDemand.push_back(-12);
	_allStationDemand.push_back(-11);
}

void TspBase::getDemand(){
	int num = _stationNum;
	int j = 0;
	for (int i = 0; i < num; i++){
		if (_allStationDemand[i] == 0){
			_mapAllToPart.push_back(-1);
			_stationNum--;
			continue;
		}
		_stationDemand.push_back(_allStationDemand[i]);
		_mapAllToPart.push_back(j++);
		_mapPartToAll.push_back(i);
	}

	delete g;
	delete cost;
	delete pos;
	g = new FullGraph(_stationNum);
	cost = new DoubleEdgeMap(*g);
	pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);

	checkDemand();
}

void TspBase::checkDemand(){
	int sum = 0;
	for (int i = 0; i < _stationNum; i++){
		sum += _allStationDemand[i];
	}
	if (sum != 0){
		cout << "Sum of demand wrong!!!" << endl;
	}
	else{
		//cout << "Sum of demand equal zero." << endl;
	}
}

void TspBase::getRandomPoints(){
	srand((unsigned)time(NULL));
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
		point poss;
		poss.a = rand() % POINT_RANGE;
		poss.b = rand() % POINT_RANGE;
		(*pos)[u] = dim2::Point<double>(poss.a, poss.b);
		_point.push_back(poss);
	}
}

void TspBase::getInputPoints(){
	point temp;
	
	temp = { 120.136037, 30.282492 };
	_point.push_back(temp);
	temp = { 120.151129, 30.278562 };
	_point.push_back(temp);
	temp = { 120.143008, 30.274695 };
	_point.push_back(temp);
	temp = { 120.153824, 30.273884 };
	_point.push_back(temp);
	temp = { 120.163741, 30.276129 };
	_point.push_back(temp);
	temp = { 120.142577, 30.286671 };
	_point.push_back(temp);
	temp = { 120.154435, 30.285049 };
	_point.push_back(temp);
	temp = { 120.160184, 30.272886 };
	_point.push_back(temp);
	temp = { 120.162519, 30.28561 };
	_point.push_back(temp);
	temp = { 120.156195, 30.277689 };
	_point.push_back(temp);
	temp = { 120.147787, 30.271576 };
	_point.push_back(temp);
}

void TspBase::getPoints(){
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		(*pos)[u] = dim2::Point<double>(_point[i].a, _point[i].b);
	}
	PRINTFPoints
}

void TspBase::getInputCost(){
	for (int i = 0; i <= _stationNum; i++){
		vector<double> temp;
		for (int j = 0; j <= _stationNum; j++){
			temp.push_back(0);
		}
		_cost.push_back(temp);
	}
	// _cost[0] = { 0, 2395, 1389, 2596, 3424, 1467, 3437, 3747, 6006, 2380, 2370, 1432 };
	// _cost[1] = { 2329, 0, 1925, 1308, 2439, 2115, 2149, 2459, 4718, 1092, 2006, 3173 };
	// _cost[2] = { 2152, 2093, 0, 2342, 2711, 1938, 3183, 2731, 5752, 2126, 932, 2301 };
	// _cost[3] = { 2436, 2157, 1738, 0, 1914, 2222, 1893, 2272, 4496, 836, 1819, 2986 };
	// _cost[4] = { 3907, 3412, 2996, 1622, 0, 3761, 2287, 1977, 3564, 1799, 2358, 4244 };
	// _cost[5] = { 2766, 2487, 3083, 2945, 3520, 0, 1954, 4096, 4935, 2729, 3643, 4229 };
	// _cost[6] = { 1940, 1661, 2257, 2119, 1867, 1726, 0, 3270, 4111, 1903, 3205, 3403 };
	// _cost[7] = { 3672, 3393, 2974, 1292, 1600, 3458, 2763, 0, 4622, 2072, 2022, 4126 };
	// _cost[8] = { 2896, 2617, 3213, 2621, 2062, 2682, 1364, 2931, 0, 2238, 3357, 4359 };
	// _cost[9] = { 2807, 2312, 1896, 1311, 1133, 2661, 1108, 2002, 3715, 0, 2020, 3144 };
	// _cost[10] = { 2412, 1566, 1501, 1381, 1858, 2449, 3011, 1878, 4870, 1584, 0, 2471 };
	// _cost[11] = { 1594, 2847, 1841, 3048, 3876, 2694, 3889, 4199, 6458, 2832, 2332, 0 };
}

void TspBase::getCost(){

	for (int i=0;i<g->nodeNum();i++)
	{
		for (int j=0;j<g->nodeNum();j++)
		{
			if (i != j){
				(*cost)[(*g).edge((*g)(i), (*g)(j))] = _cost[i][j];
			}
		}
		//cout<<endl;
	}
	//cout<<endl;
	//{
	//	for (int i=0;i<g->nodeNum();i++)
	//	{
	//		for (int j=0;j<g->nodeNum();j++)
	//		{
	//			if (i != j){
	//				cout << (*cost)[(*g).edge((*g)(i), (*g)(j))] << " ";
	//			}
	//			else{
	//				cout << "0 ";
	//			}
	//			
	//		}
	//		cout<<endl;
	//	}
	//	cout<<endl;
	//}
	
}

void TspBase::getRandomCost(){
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		int j = 0;
		vector<double> costrow;
		for (FullGraph::NodeIt v(*g); v != INVALID; ++v, ++j) {
			if (u != v) {
				//(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = (double)sqrt(((*pos)[u] - (*pos)[v]).normSquare());
				costrow.push_back((double)sqrt(((*pos)[u] - (*pos)[v]).normSquare()));
			}
			else{
				costrow.push_back(0);
			}
		}
		costrow.push_back((double)sqrt(((*pos)[u]).normSquare()));
		_cost.push_back(costrow);
	}

	// for depot:last row
	vector<double> costrow;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		costrow.push_back((double)sqrt(((*pos)[u]).normSquare()));
	}
	costrow.push_back(0);
	_cost.push_back(costrow);

	// PRINTFCost
}

// Get a Random TSP sequence
// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
template <typename TSP>
void TspBase::getTspTour(const std::string &alg_name) {
	GRAPH_TYPEDEFS(FullGraph);

	TSP alg(*g, *cost);
	Opt2Tsp<DoubleEdgeMap > alg2(*g,*cost);
	std::vector<Node> s1;
	alg.run();
	alg.tourNodes(std::back_inserter(s1));
	_tspSum = alg2.run(s1);
	std::vector<Node> vec;
	alg2.tourNodes(std::back_inserter(vec));
	for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		FullGraph::Node node = *it;
		_path.push_back((*g).index(node));
	}

	PRINTFTSPtour
}

void TspBase::data(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	cout << endl;
	cout << "vehicle capacity:" << VEHICLE_CAPACITY << endl;
	cout << "station capacity:" << STATION_CAPACITY << endl;
	cout << endl;

	start = clock();
	getDemand();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetRandomDemand:" << totaltime << "ms!" << endl;

	start = clock();
	getPoints();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetRandomPoints:" << totaltime << "ms!" << endl;

	start = clock();
	getCost();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetCost:" << totaltime << "ms!" << endl;

	getTspTour<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
}

void TspBase::randomData(){

	clock_t start, finish, sum;
	double totaltime, totaltime0, totaltime1, totaltime2;
	sum = clock();

	start = clock();
	getRandomDemand();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetRandomDemand:" << totaltime << "ms!" << endl;

	start = clock();
	getRandomPoints();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetRandomPoints:" << totaltime << "ms!" << endl;

	start = clock();
	getRandomCost();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC * 1000;
	//cout << "\ngetCost:" << totaltime << "ms!" << endl;

}

void TspBase::inputData(){
	getInputDemand();
	getInputPoints();
	getInputCost();
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
