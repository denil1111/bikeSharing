
#include "supernode.h"

SuperNode::SuperNode(int excessnumber){
	_excessNumber = excessnumber;
	_numberOfZeroPieceBehind = 0;
	_numberOfZeroPieceFront = 0;

}

SuperNode::~SuperNode(){
}

int SuperNode::getASuperNode(vector<int> &path, vector<int>::iterator &currentIt, vector<int> &demand, int surplusdemand, int startnode, int &stopflag, int stationnum){
	int nowPieceTypeFlag;

	double sum = 0;

	if (demand[*currentIt] >= 0){
		nowPieceTypeFlag = PIECE_P;
	}
	else{
		nowPieceTypeFlag = PIECE_N;
	}

	if (surplusdemand != 0){
		if (surplusdemand > _excessNumber){
			_pieceTypeFlag = PIECE_P;
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = _excessNumber;
			_stationidAndDemand.push_back(temp);

			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand < -_excessNumber){
			_pieceTypeFlag = PIECE_N;
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = -_excessNumber;
			_stationidAndDemand.push_back(temp);

			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand == _excessNumber){
			_pieceTypeFlag = PIECE_P;
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = _excessNumber;
			_stationidAndDemand.push_back(temp);

			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			++stopflag;
			return 0;
		}
		else if (surplusdemand == -_excessNumber){
			_pieceTypeFlag = PIECE_N;
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = -_excessNumber;
			_stationidAndDemand.push_back(temp);

			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			++stopflag;
			return 0;
		}
		else{
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = surplusdemand;
			_stationidAndDemand.push_back(temp);

			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			++stopflag;
			sum += surplusdemand;
		}

		
	}

	while (stopflag < stationnum){	
		sum += demand[*currentIt];

		if (nowPieceTypeFlag == PIECE_P && sum <= 0){
			_pieceTypeFlag = PIECE_0;

			if (sum == 0){
				StationidAndDemand temp;
				temp.stationId = *currentIt;
				temp.stationDemand = demand[*currentIt];
				_stationidAndDemand.push_back(temp);
				
				if ((++currentIt) == path.end()){
					currentIt = path.begin();
				}
				++stopflag;
				return 0;
			}
			else{
				StationidAndDemand temp;
				temp.stationId = *currentIt;
				temp.stationDemand = demand[*currentIt] - sum;
				_stationidAndDemand.push_back(temp);
				return sum;
			}
		}
		else if (nowPieceTypeFlag == PIECE_N && sum >= 0){
			_pieceTypeFlag = PIECE_0;

			if (sum == 0){
				++stopflag;

				StationidAndDemand temp;
				temp.stationId = *currentIt;
				temp.stationDemand = demand[*currentIt];
				_stationidAndDemand.push_back(temp);
				if ((++currentIt) == path.end()){
					currentIt = path.begin();
				}
				return 0;
			}
			else{
				StationidAndDemand temp;
				temp.stationId = *currentIt;
				temp.stationDemand = demand[*currentIt] - sum;
				_stationidAndDemand.push_back(temp);
				return sum;
			}
		}
		// needn't cut:
		else if (sum == _excessNumber){
			_pieceTypeFlag = PIECE_P;
			++stopflag;

			StationidAndDemand temp; 
//			temp.isCutPoint = PIECE_NULL;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt];
			_stationidAndDemand.push_back(temp);
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// need cut:
		else if (sum > _excessNumber){
			_pieceTypeFlag = PIECE_P;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_P;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt] - (sum - _excessNumber);
			_stationidAndDemand.push_back(temp);
			return sum - _excessNumber;
		}
		// needn't cut:
		else if (sum == -_excessNumber){
			_pieceTypeFlag = PIECE_N;
			++stopflag;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_NULL;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt];
			_stationidAndDemand.push_back(temp);
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// need cut:
		else if (sum < -_excessNumber){
			_pieceTypeFlag = PIECE_N;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_N;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt] - (sum + _excessNumber);
			_stationidAndDemand.push_back(temp);
			return sum + _excessNumber;
		}
		++stopflag;

		StationidAndDemand temp;
//		temp.isCutPoint = PIECE_NULL;
		temp.stationId = *currentIt;
		temp.stationDemand = demand[*currentIt];
		_stationidAndDemand.push_back(temp);
		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}

	}// while
	// last one may be a zero piece:
	if (*currentIt == startnode){
		//if ((currentIt) == path.begin()){
		//	_endIt = path.end() - 1;
		//}
		//else{
		//	_endIt = currentIt - 1;
		//}
		_pieceTypeFlag = PIECE_0;

		StationidAndDemand temp;
//		temp.isCutPoint = PIECE_NULL;
		temp.stationId = *currentIt;
		temp.stationDemand = demand[*currentIt];
		_stationidAndDemand.push_back(temp);
		cout << "The sum should be :" << sum << endl;
		return 0;
	}
}

int SuperNode::getASuperNodeNoZero(vector<int> &path, vector<int>::iterator &currentIt, vector<int> &demand, int surplusdemand, int startnode, int &stopflag, int stationnum){

	double sum = 0;

	if (surplusdemand != 0){
		if (surplusdemand > _excessNumber){
			_pieceTypeFlag = PIECE_P;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_P;
			temp.stationId = *currentIt;
			temp.stationDemand = _excessNumber;
			_stationidAndDemand.push_back(temp);

			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand < -_excessNumber){
			_pieceTypeFlag = PIECE_N;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_N;
			temp.stationId = *currentIt;
			temp.stationDemand = -_excessNumber;
			_stationidAndDemand.push_back(temp);

			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand == _excessNumber){
			_pieceTypeFlag = PIECE_P;
			++stopflag;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_P;
			temp.stationId = *currentIt;
			temp.stationDemand = _excessNumber;
			_stationidAndDemand.push_back(temp);

			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		else if (surplusdemand == -_excessNumber){
			_pieceTypeFlag = PIECE_N;
			++stopflag;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_N;
			temp.stationId = *currentIt;
			temp.stationDemand = -_excessNumber;
			_stationidAndDemand.push_back(temp);

			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		sum += surplusdemand;
		++stopflag;

		StationidAndDemand temp;
		if (surplusdemand > 0){
//			temp.isCutPoint = PIECE_P;
		}
		else{
//			temp.isCutPoint = PIECE_N;
		}
		temp.stationId = *currentIt;
		temp.stationDemand = surplusdemand;
		_stationidAndDemand.push_back(temp);

		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}
	}

	while (stopflag < stationnum){
		sum += demand[*currentIt];

		// needn't cut:
		if (sum == _excessNumber){
			_pieceTypeFlag = PIECE_P;
			++stopflag;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_NULL;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt];
			_stationidAndDemand.push_back(temp);
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// need cut:
		else if (sum > _excessNumber){
			_pieceTypeFlag = PIECE_P;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_P;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt] - (sum - _excessNumber);
			_stationidAndDemand.push_back(temp);
			return sum - _excessNumber;
		}
		// needn't cut:
		else if (sum == -_excessNumber){
			_pieceTypeFlag = PIECE_N;
			++stopflag;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_NULL;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt];
			_stationidAndDemand.push_back(temp);
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// need cut:
		else if (sum < -_excessNumber){
			_pieceTypeFlag = PIECE_N;

			StationidAndDemand temp;
//			temp.isCutPoint = PIECE_N;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt] - (sum + _excessNumber);
			_stationidAndDemand.push_back(temp);
			return sum + _excessNumber;
		}
		++stopflag;

		StationidAndDemand temp;
//		temp.isCutPoint = PIECE_NULL;
		temp.stationId = *currentIt;
		temp.stationDemand = demand[*currentIt];
		_stationidAndDemand.push_back(temp);
		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}

	}// while
	// last one may be a zero piece:
	if (*currentIt == startnode){

		_pieceTypeFlag = PIECE_0;

		//StationidAndDemand temp;
		//temp.isCutPoint = PIECE_NULL;
		//temp.stationId = *currentIt;
		//temp.stationDemand = demand[*currentIt];
		//_stationidAndDemand.push_back(temp);
		//cout << "The sum should be :" << sum << endl;
		return 0;
	}
}
