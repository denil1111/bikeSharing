/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef UNCAPACITATED_BSP_H
#define UNCAPACITATED_BSP_H

#include "bspbase.h"


class UncapacitatedBSP:public BspBase
{

private:

	vector<int> _positiveStationVisiteFlag;
	int _startStationUncapacitatedBSP;

public:
	UncapacitatedBSP(TspBase &tspbase);
	~UncapacitatedBSP();



	//bool isExistNotVisitedPositiveStation();
	//bool isAPositiveStation(int number);
	void getPath();
	//int  getStartStation();

	//void revertPath();
	void run();

};

#endif
