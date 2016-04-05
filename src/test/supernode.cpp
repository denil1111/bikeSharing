/*
*
*
*/

#include "supernode.h"

SuperNode::SuperNode(int excessnumber){

	_zeroCostPointNumber = 0;
	_numberOfZeroPieceBehind = 0;

	_excessNumber = excessnumber;
	_sum = 0;
	_isHaveCutNode = false;
	_firstNodeDemand = 0;
	_lastNodeDemand = 0;
	_nodeNumberInSuperNode = 0;
	flag = true;
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
			//cout << "5 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			//cout << "  剩余量" << surplusdemand - _excessNumber << endl;
			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand < -_excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = -_excessNumber;
			//cout << "6 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			//cout << "  剩余量" << surplusdemand + _excessNumber <<endl;
			return surplusdemand + _excessNumber;
		}
		else if (surplusdemand == _excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = _excessNumber;
			++stopflag;
			//cout << "stopflag:" << stopflag << endl;
			//cout << "7 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
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
			//cout << "stopflag:" << stopflag << endl;
			//cout << "8 sum = " << _sum << " start = " << *_startIt <<" end = " << *currentIt << endl;
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		sum += surplusdemand;
		++_nodeNumberInSuperNode;
		++stopflag;
		//cout << "stopflag:" << stopflag << endl;
		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}
	}

	//while (*currentIt != startnode || flag){
	while (stopflag < stationnum){
		flag = false;
		sum += demand[*currentIt];
		++_nodeNumberInSuperNode;
		
		if (_nowPieceTypeFlag == PIECE_P && sum <= 0){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_0;
			_sum = 0;
			_isHaveCutNode = true;
			_lastNodeDemand = sum;
			//cout << "10 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if (sum == 0){
				++stopflag;
				//cout << "stopflag:" << stopflag << endl;
				if ((++currentIt) == path.end()){
					currentIt = path.begin();
				}
				return 0;
			}
			else{
				return sum;
			}
		}
		else if (_nowPieceTypeFlag == PIECE_N && sum >= 0){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_0;
			_sum = 0;
			_isHaveCutNode = true;
			_lastNodeDemand = sum;
			//cout << "10 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if (sum == 0){
				++stopflag;
				//cout << "stopflag:" << stopflag << endl;
				if ((++currentIt) == path.end()){
					currentIt = path.begin();
				}
				return 0;
			}
			else{
				return sum;
			}
		}
		// 如果刚好等于，那最好啦，不用分割啦：
		else if (sum == _excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = sum;
			++stopflag;
			//cout << "stopflag:" << stopflag << endl;
			//cout << "1 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// 这种情况下是需要分割的：
		else if (sum > _excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = _excessNumber;
			_isHaveCutNode = true;
			_lastNodeDemand = demand[*currentIt] - (sum - _excessNumber);
			//cout << "2 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			return sum - _excessNumber;
		}
		// 这个也不需要分割：
		else if (sum == -_excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = sum;
			++stopflag;
			//cout << "stopflag:" << stopflag << endl;
			//cout << "3 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// 这个也需要分割的：
		else if(sum < -_excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = -_excessNumber;
			_isHaveCutNode = true;
			_lastNodeDemand = demand[*currentIt] - (sum - _excessNumber);
			//cout << "4 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			return sum + _excessNumber;
		}
		++stopflag;
		//cout << "stopflag:" << stopflag << endl;
		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}

	}// while
	if (*currentIt == startnode){
		if ((currentIt) == path.begin()){
			_endIt = path.end() - 1;
		}
		else{
			_endIt = currentIt - 1;
		}
		_pieceTypeFlag = PIECE_0;
		_sum = 0;
		//cout << "这里的sum应该是：" << sum << endl;
		return 0;
	}
}

int SuperNode::getASuperNode(vector<int> &path, vector<int> &demand, int startnode, vector<int>::iterator &currentIt, int surplusdemand){
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
			//cout << "5 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			//cout << "  last demand" << surplusdemand - _excessNumber << endl;
			return surplusdemand - _excessNumber;
		}
		else if (surplusdemand < -_excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = -_excessNumber;
			//cout << "6 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			//cout << "  last demand" << surplusdemand + _excessNumber <<endl;
			return surplusdemand + _excessNumber;
		}
		else if (surplusdemand == _excessNumber){
			++_nodeNumberInSuperNode;
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = _excessNumber;
			//cout << "7 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
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
			//cout << "8 sum = " << _sum << " start = " << *_startIt <<" end = " << *currentIt << endl;
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		sum += surplusdemand;
		++_nodeNumberInSuperNode;
		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}
	}

	while (*currentIt != startnode || flag){
		flag = false;
		sum += demand[*currentIt];
		++_nodeNumberInSuperNode;

		if (_nowPieceTypeFlag == PIECE_P && sum <= 0){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_0;
			_sum = 0;
			_isHaveCutNode = true;
			_lastNodeDemand = sum;
			//cout << "10 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if (sum == 0){
				if ((++currentIt) == path.end()){
					currentIt = path.begin();
				}
				return 0;
			}
			else{
				return sum;
			}
		}
		else if (_nowPieceTypeFlag == PIECE_N && sum >= 0){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_0;
			_sum = 0;
			_isHaveCutNode = true;
			_lastNodeDemand = sum;
			//cout << "10 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if (sum == 0){
				if ((++currentIt) == path.end()){
					currentIt = path.begin();
				}
				return 0;
			}
			else{
				return sum;
			}
		}
		// 如果刚好等于，那最好啦，不用分割啦：
		else if (sum == _excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = sum;
			//cout << "1 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// 这种情况下是需要分割的：
		else if (sum > _excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_P;
			_sum = _excessNumber;
			_isHaveCutNode = true;
			_lastNodeDemand = demand[*currentIt] - (sum - _excessNumber);
			//cout << "2 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			return sum - _excessNumber;
		}
		// 这个也不需要分割：
		else if (sum == -_excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = sum;
			//cout << "3 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			if ((++currentIt) == path.end()){
				currentIt = path.begin();
			}
			return 0;
		}
		// 这个也需要分割的：
		else if (sum < -_excessNumber){
			_endIt = currentIt;
			_pieceTypeFlag = PIECE_N;
			_sum = -_excessNumber;
			_isHaveCutNode = true;
			_lastNodeDemand = demand[*currentIt] - (sum - _excessNumber);
			//cout << "4 sum = " << _sum << " start = " << *_startIt << " end = " << *currentIt << endl;
			return sum + _excessNumber;
		}
		if ((++currentIt) == path.end()){
			currentIt = path.begin();
		}

	}// while
	if (*currentIt == startnode){
		if ((currentIt) == path.begin()){
			_endIt = path.end() - 1;
		}
		else{
			_endIt = currentIt - 1;
		}
		_pieceTypeFlag = PIECE_0;
		_sum = 0;
		//cout << "这里的sum应该是：" << sum << endl;
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

int SuperNode::getFirstNodeDemand(){
	return _firstNodeDemand;
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