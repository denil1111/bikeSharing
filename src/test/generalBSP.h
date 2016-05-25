#ifndef GENERAL_BSP_H
#define GENERAL_BSP_H

#include "bspbase.h"

class GeneralBSP :public BspBase
{
public:

	GeneralBSP(TspBase &tspbase);
	~GeneralBSP();

	int getStartStation(vector<StationidAndDemand> &mincostpath, vector<StationidAndDemand> &resultpath, int &minSum);
	void run();

};

#endif
