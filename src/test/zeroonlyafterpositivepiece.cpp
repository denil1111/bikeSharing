#include "zeroonlyafterpositivepiece.h"

ZeroOnlyAfterPositivePiece::ZeroOnlyAfterPositivePiece(TspBase &tspbase) :BspBase(tspbase){
	capacityFlag = true;
}

ZeroOnlyAfterPositivePiece::~ZeroOnlyAfterPositivePiece(){

}

void ZeroOnlyAfterPositivePiece::run(){

	//cout << endl << "ZeroOnlyAfterPositivePiece:" << endl;

	for (int i = 0; i < _tspBase._stationNum; i++){
		getSuperNodePiecesZeroOnlyAfterPositive(i);
		calculateMinCostAmongSuperNode();
		machingSuperNode();
		getPath();
	}
	//int i = 0;
	//getSuperNodePiecesZeroOnlyAfterPositive(i);
	//calculateMinCostAmongSuperNode();
	//machingSuperNode();
	//getPath();


	//	_minSum = getFinalSum(_minCostPath);


	//PRINTFFinalPath
	//PRINTFSuperNodeInformation

	//cout << "ZeroOnlyAfterPositivePiece sum cost:" << _minSum << endl;

}