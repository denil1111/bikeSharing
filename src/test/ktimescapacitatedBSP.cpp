#include"ktimescapacitatedBSP.h"

KTimesCapacitatedBSP::KTimesCapacitatedBSP(TspBase &tspbase):BspBase(tspbase){
	capacityFlag = true;
}

KTimesCapacitatedBSP::~KTimesCapacitatedBSP(){

}

void KTimesCapacitatedBSP::run(){
   // _tspBase.data();

	cout << endl << "ktimescapacitatedBSP:" << endl;

	for (int i = 0; i < _tspBase._stationNum; i++){
	//int i = 0;
	getSuperNodePieces(i);
		calculateMinCostAmongSuperNode();
		machingSuperNode();
		getPath();
		
		//cout << "Get a path " << i << endl << endl;
	}
	_minSum = getFinalSum(_minCostPath);
//	mapPath();
	
	PRINTFFinalPath
	PRINTFSuperNodeInformation

	cout << "ktimescapacitatedBSP sum cost:" << _minSum << endl;

}