/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef NOZEROCAPACITATED_BSP_H
#define NOZEROCAPACITATED_BSP_H

#include "bspbase.h"

class NoZeroCapacitatedBSP:public BspBase
{

public:

	NoZeroCapacitatedBSP(TspBase &tspbase);
	~NoZeroCapacitatedBSP();

	virtual void calculateMinCostOfTwoSuperNode(int first, int second);
	virtual void calculateMinCostAmongSuperNode();

	virtual void getOrderPath(vector<SuperNode>, vector<SuperNode>);
	virtual void getReversePath(vector<SuperNode>, vector<SuperNode>);

	void run();
};

#endif
