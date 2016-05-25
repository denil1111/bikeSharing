#include "debug.h"

Debug::Debug(){
}

Debug::~Debug(){
}

bool Debug::checkDemand(vector<int> demand, int stationcapacity){
	int sum = 0;
	for (vector<int>::iterator it = demand.begin(); it < demand.end(); ++it){
		if (abs(*it) > stationcapacity){
			cout << "abs(demand) exceed station capacitity" << endl;
			return false;
		}
		sum += *it;
	}
	if (sum == 0){
		//cout << "demand OK!" << endl;
		return true;
	}
	else{
		cout << "Sum of demand wrong!!!" << endl;
		return false;
	}
}

bool Debug::checkCost(vector<vector<double> > cost){
	for (int i = 0; i < cost.size(); i++){

	}

	return false;
}