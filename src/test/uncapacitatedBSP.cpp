#include"uncapacitatedBSP.h"


UncapacitatedBSP::UncapacitatedBSP(TspBase &tspbase):BspBase(tspbase){
	capacityFlag = false;
}

UncapacitatedBSP::~UncapacitatedBSP(){

}

void UncapacitatedBSP::getPath(){
	vector<StationidAndDemand> tempVector;
	for (vector<int>::iterator it = _tspBase._path.begin(); it < _tspBase._path.end(); ++it){
		StationidAndDemand temp;
		temp.stationId = *it;
		temp.stationDemand = _tspBase._stationDemand[*it];
		tempVector.push_back(temp);
	}

	getStartStation(tempVector, _minCostPath, _minSum);
}


void UncapacitatedBSP::run(){

	//_tspBase.data();

	cout << "UncapacitatedBSP:" << endl;

	getPath();

	PRINTFFinalPath

	cout << "Uncapacitated BSP sum cost:" << _tspBase._tspSum << endl << endl;

}
