#include"ktimescapacitatedBSP.h"

KTimesCapacitatedBSP::KTimesCapacitatedBSP(TspBase &tspbase):BspBase(tspbase){
	capacityFlag = true;
}

KTimesCapacitatedBSP::~KTimesCapacitatedBSP(){

}

void KTimesCapacitatedBSP::run(){

	cout << endl << "ktimescapacitatedBSP:" << endl;

	//for (int i = 0; i < _tspBase._stationNum; i++){
	//	getSuperNodePieces(i);
	//	calculateMinCostAmongSuperNode();
	//	machingSuperNode();
	//	getPath();
	//}
	int i = 9;
	getSuperNodePieces(i);
	calculateMinCostAmongSuperNode();
	machingSuperNode();
	getPath();


//	_minSum = getFinalSum(_minCostPath);

	
	PRINTFFinalPath
	PRINTFSuperNodeInformation

	cout << "ktimescapacitatedBSP sum cost:" << _minSum << endl;

}