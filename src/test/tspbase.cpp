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

// random:�� -STATION_CAPACITY��STATION_CAPACITY ��
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

void TspBase::getPoints(){
	int i = 0;
	for (FullGraph::NodeIt u(*g); u != INVALID; ++u, ++i) {
		(*pos)[u] = dim2::Point<double>(_point[i].a, _point[i].b);
	}
	PRINTFPoints
}

void TspBase::getCost(){
	int i = 0;
	cout<<"_mapAllToPart:";
	for (int i=0;i<_mapAllToPart.size();i++)
	{
		cout<<_mapAllToPart[i]<<",";
	}
	cout<<endl;
	for (int i=0;i<g->nodeNum();i++)
	{
		for (int j=0;j<g->nodeNum();j++)
		{
			(*cost)[(*g).edge((*g)(i), (*g)(j))] = _cost[i][j];
		}
		cout<<endl;
	}
	cout<<endl;
	{
		for (int i=0;i<g->nodeNum();i++)
		{
			for (int j=0;j<g->nodeNum();j++)
			{
				cout<<(*cost)[(*g).edge((*g)(i), (*g)(j))]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	
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
