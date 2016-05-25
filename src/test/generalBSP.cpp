#include "generalBSP.h"

GeneralBSP::GeneralBSP(TspBase &tspbase) :BspBase(tspbase){
	capacityFlag = true;
}

GeneralBSP::~GeneralBSP(){

}

int GeneralBSP::getStartStation(vector<StationidAndDemand> &mincostpath, vector<StationidAndDemand> &resultpath, int &minSum){
	vector<StationidAndDemand>::const_iterator it = mincostpath.begin();
	vector<StationidAndDemand>::const_iterator itt = mincostpath.begin();
	_startPoint = -1;

	deleteRepeatStationPoint(mincostpath);
	if (checkSum(mincostpath)){
		for (int i = 0; i<mincostpath.size(); i++)
		{
			int tempSum = mincostpath[i].stationDemand;
			int tempNum = i;
			bool flag = false;

			vector<StationidAndDemand> temppath;
			for (vector<StationidAndDemand>::iterator it = mincostpath.begin(); it < mincostpath.end(); ++it){
				temppath.push_back(*it);
			}

			while (tempSum >= 0 && tempSum <= _tspBase.VEHICLE_CAPACITY)
			{
				if (tempNum == i && flag == true) {
					_startPoint = i;
					revertPath(temppath);
					//if (capacityFlag == true){
					//	tryToMeetPositive(temppath);
					//	tryToMeetNegative(temppath);
					//}
					mapPath(temppath);
					_pathSet.push_back(temppath);

					int costSum = getFinalSum(temppath);
					if (costSum < minSum){
						resultpath.clear();
						for (vector<StationidAndDemand>::iterator it = temppath.begin(); it < temppath.end(); ++it){
							resultpath.push_back(*it);
						}
						minSum = costSum;
						_finalSuperNodeVector_PIECE_P.clear();
						_finalSuperNodeVector_PIECE_N.clear();
						_finalSuperNodeVector_PIECE_0.clear();
						for (int i = 0; i < _superNodeVector_PIECE_P.size(); i++){
							_finalSuperNodeVector_PIECE_P.push_back(_superNodeVector_PIECE_P[i]);
						}
						for (int i = 0; i < _superNodeVector_PIECE_N.size(); i++){
							_finalSuperNodeVector_PIECE_N.push_back(_superNodeVector_PIECE_N[i]);
						}
						for (int i = 0; i < _superNodeVector_PIECE_0.size(); i++){
							_finalSuperNodeVector_PIECE_0.push_back(_superNodeVector_PIECE_0[i]);
						}
					}
					return _startPoint;
				}
				if (flag == false){
					flag = true;
				}
				tempNum = (tempNum + 1) % temppath.size();
				tempSum = tempSum + temppath[tempNum].stationDemand;
			}
			if (tempSum > _tspBase.VEHICLE_CAPACITY){
				cout << "i" << i << ":" << tempSum << ":" << tempNum << "   ";
			}
		}
		//cout << endl;
	}

	return _startPoint;
}

void GeneralBSP::run(){

	//cout << endl << "GeneralBSP:" << endl;

	//for (int i = 0; i < _tspBase._stationNum; i++){
	//	getSuperNodePiecesZeroOnlyAfterPositive(i);
	//	calculateMinCostAmongSuperNode();
	//	machingSuperNode();
	//	getPath();
	//}
	int i = 0;
	getSuperNodePiecesZeroOnlyAfterPositive(i);
	calculateMinCostAmongSuperNode();
	machingSuperNode();
	getPath();


	//	_minSum = getFinalSum(_minCostPath);


	//PRINTFFinalPath
	//PRINTFSuperNodeInformation

	//cout << "GeneralBSP sum cost:" << _minSum << endl;

}