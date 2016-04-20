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
	void runRandom();


public:

	NoZeroCapacitatedBSP(TspBase &tspbase);
	~NoZeroCapacitatedBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	void calculateMinCostOfTwoSuperNode(int first, int second);

	void getPathBeginPositive();
	void getPathBeginNegative();
	void getPathBeginPositiveReverse();
	void getPathBeginNegativeReverse();

	// calculate cost among two super node:
	void beginPositive(int positivesupernode, int negativesupernode);
	void beginPositiveReverse(int positivesupernode, int negativesupernode);
	void beginNegative(int positivesupernode, int negativesupernode);
	void beginNegativeReverse(int positivesupernode, int negativesupernode);
};

#endif
