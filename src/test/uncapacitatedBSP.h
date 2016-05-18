/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef UNCAPACITATED_BSP_H
#define UNCAPACITATED_BSP_H

#include "bspbase.h"


class UncapacitatedBSP:public BspBase
{

public:
	UncapacitatedBSP(TspBase &tspbase);
	~UncapacitatedBSP();

	void getPath();
	int  getStartStation(vector<StationidAndDemand> &mincostpath, vector<StationidAndDemand> &resultpath, int &minSum);
	void run();

};

#endif
