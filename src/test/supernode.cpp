/*
*
*
*/

#include "supernode.h"

SuperNode::SuperNode(int excessnumber){

	_zeroCostPointNumber = 0;
	_numberOfZeroPieceBehind = 0;
	_numberOfZeroPieceFront = 0;

	_excessNumber = excessnumber;
	_sum = 0;
	_lastNodeDemand = 0;
	_nodeNumberInSuperNode = 0;
	flag = true;
	
	_isCutNodeInFront = false;
	_isCutNodeInBehind = false;
	_demandInFront = 0;
	_demandInBehind = 0;
}

void SuperNode::setsuperNodeNumber(int supernodeId){
	_superNodeId = supernodeId;
}

void SuperNode::setMatchingNumber(int number){
	_matchingNumber = number;
}

void SuperNode::setMinCostMatchingNumber(int number){
	_minCostMatchingNumber = number;
}

void SuperNode::setNumberOfZeroPieceFront(int number){
	_numberOfZeroPieceFront = number;
}

int& SuperNode::getNumberOfZeroPieceFront(){
	return _numberOfZeroPieceFront;
}

int& SuperNode::getNumberOfZeroPieceBehind(){
	return _numberOfZeroPieceBehind;
}

int SuperNode::getASuperNode(vector<int> &path, vector<int> &demand, int startnode, vector<int>::iterator &currentIt, int surplusdemand, int &stopflag, int stationnum){
	_startIt = currentIt;

	double sum = 0;

	if (demand[*currentIt] >= 0){
		_nowPieceTypeFlag = PIECE_P;
	}
	else{
		_nowPieceTypeFlag = PIECE_N;
	}

	if (surplusdemand != 0){
		if (surplusdemand > _excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = _excessNumber;

			_isCutNodeInFront = true;
			_isCutNodeInBehind = true;
			_demandInFront = _excessNumber;
			_demandInBehind = _excessNumber;

			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = _excessNumber;
			_stationidAndDemand.push_back(temp);

			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand < -_excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = -_excessNumber;

			_isCutNodeInFront = true;
			_isCutNodeInBehind = true;
			_demandInFront = -_excessNumber;
			_demandInBehind = -_excessNumber;
			
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = -_excessNumber;
			_stationidAndDemand.push_back(temp);
	
			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand == _excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = _excessNumber;
			++stopflag;
			
			_isCutNodeInFront = true;
			_isCutNodeInBehind = true;
			_demandInFront = _excessNumber;
			_demandInBehind = _excessNumber;
			
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = _excessNumber;
			_stationidAndDemand.push_back(temp);

			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		else if (surplusdemand == -_excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = -_excessNumber;
			++stopflag;
			
			_isCutNodeInFront = true;
			_isCutNodeInBehind = true;
			_demandInFront = -_excessNumber;
			_demandInBehind = -_excessNumber;
			
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = -_excessNumber;
			_stationidAndDemand.push_back(temp);

			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		sum += surplusdemand;
		++_nodeNumberInSuperNode;
		++stopflag;

		_isCutNodeInFront = true;
		_demandInFront = surplusdemand;

		StationidAndDemand temp;
		temp.stationId = *currentIt;
		temp.stationDemand = surplusdemand;
		_stationidAndDemand.push_back(temp);

		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}
	}

	while (stopflag < stationnum){
		flag = false;
		sum += demand[*currentIt];
		++_nodeNumberInSuperNode;
		
		if (_nowPieceTypeFlag == PIECE_P && sum <= 0){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_0;
			_sum = 0;
			_lastNodeDemand = sum;

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
				_isCutNodeInBehind = true;
				_demandInBehind = demand[*currentIt] - sum;

				StationidAndDemand temp;
				temp.stationId = *currentIt;
				temp.stationDemand = demand[*currentIt] - sum;
				_stationidAndDemand.push_back(temp);
				return sum;
			}
		}
		else if (_nowPieceTypeFlag == PIECE_N && sum >= 0){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_0;
			_sum = 0;
			_lastNodeDemand = sum;

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
				_isCutNodeInBehind = true;
				_demandInBehind = demand[*currentIt] - sum;

				StationidAndDemand temp;
				temp.stationId = *currentIt;
				temp.stationDemand = demand[*currentIt] - sum;
				_stationidAndDemand.push_back(temp);
				return sum;
			}
		}
		// needn't cut:
		else if (sum == _excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = sum;
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
		// need cut:
		else if (sum > _excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = _excessNumber;
			_lastNodeDemand = demand[*currentIt] - (sum - _excessNumber);

			_isCutNodeInBehind = true;
			_demandInBehind = demand[*currentIt] - (sum - _excessNumber);
			
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt] - (sum - _excessNumber);
			_stationidAndDemand.push_back(temp);
			return sum - _excessNumber;
		}
		// needn't cut:
		else if (sum == -_excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = sum;
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
		// need cut:
		else if(sum < -_excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = -_excessNumber;
			_lastNodeDemand = demand[*currentIt] - (sum - _excessNumber);

			_isCutNodeInBehind = true;
			_demandInBehind = demand[*currentIt] - (sum + _excessNumber);
			
			StationidAndDemand temp;
			temp.stationId = *currentIt;
			temp.stationDemand = demand[*currentIt] - (sum + _excessNumber);
			_stationidAndDemand.push_back(temp);
			return sum + _excessNumber;
		}
		++stopflag;
		
		StationidAndDemand temp;
		temp.stationId = *currentIt;
		temp.stationDemand = demand[*currentIt];
		_stationidAndDemand.push_back(temp);
		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}

	}// while
	// last one may be a zero piece:
	if (*currentIt == startnode){
		if ((currentIt) == path.begin()){
			_endIt = path.end() - 1;
		}		else{
			_endIt = currentIt - 1;
		}
		_pieceTypeFlag = PIECE_0;
		_sum = 0;

		StationidAndDemand temp;
		temp.stationId = *currentIt;
		temp.stationDemand = demand[*currentIt];
		_stationidAndDemand.push_back(temp);
		cout << "The sum should be :" << sum << endl;
		return 0;
	}
}

int SuperNode::getSuperNodeId(){
	return _superNodeId;
}

vector<int>::iterator SuperNode::getStartIt(){
	return _startIt;
}

vector<int>::iterator SuperNode::getEndIt(){
	return _endIt;
}

int SuperNode::getMatchingNumber(){
	return _matchingNumber;
}

int SuperNode::getLastNodeDemand(){
	return _lastNodeDemand;
}

int SuperNode::getPieceTypeFlag(){
	return _pieceTypeFlag;
}

int SuperNode::getNodeNumberInSuperNode(){
	return _nodeNumberInSuperNode;
}