/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef NOZEROCAPACITATED_BSP_H
#define NOZEROCAPACITATED_BSP_H

#include "bspbase.h"

class NoZeroCapacitatedBSP:public BspBase
{

public:

	void run();


public:

	NoZeroCapacitatedBSP(TspBase &tspbase);
	~NoZeroCapacitatedBSP();

	

	/*void calculateMinCostOfTwoSuperNode(int first, int second);*/
	virtual void calculateMinCostOfTwoSuperNode(int first, int second);
	virtual void calculateMinCostAmongSuperNode();

	virtual void getPathBeginPositive();
	virtual void getPathBeginNegative();
	virtual void getPathBeginPositiveReverse();
	virtual void getPathBeginNegativeReverse();


	// calculate cost among two super node:
	void beginPositive(int positivesupernode, int negativesupernode);
	void beginPositiveReverse(int positivesupernode, int negativesupernode);
	void beginNegative(int positivesupernode, int negativesupernode);
	void beginNegativeReverse(int positivesupernode, int negativesupernode);
};

#endif
