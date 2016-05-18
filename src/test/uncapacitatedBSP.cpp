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

int UncapacitatedBSP::getStartStation(vector<StationidAndDemand> &mincostpath, vector<StationidAndDemand> &resultpath, int &minSum){
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

			while (tempSum >= 0)
			{
				if (tempNum == i && flag == true) {
					_startPoint = i;
					revertPath(temppath);
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
					break;
				}
				if (flag == false){
					flag = true;
				}
				tempNum = (tempNum + 1) % temppath.size();
				tempSum = tempSum + temppath[tempNum].stationDemand;
			}
		}
	}

	return _startPoint;
}



void UncapacitatedBSP::run(){

	//_tspBase.data();

	cout << "UncapacitatedBSP:" << endl;

	getPath();

	PRINTFFinalPath

	cout << "Uncapacitated BSP sum cost:" << _minSum << endl << endl;

}
