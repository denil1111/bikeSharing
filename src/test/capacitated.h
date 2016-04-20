/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
*/

#ifndef CAPACITATED_BSP_H
#define CAPACITATED_BSP_H

#include "bspbase.h"

class CapacitatedBSP :public BspBase
{

public:

	void run();
	void runRandom();

public:
	
	CapacitatedBSP(TspBase &tspbase);
	~CapacitatedBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);

	
	void calculateMinCostAmongSuperNode();
	void getPath();

	

	// new 
	void beginPositive(int positivesupernode, int negativesupernode);
	void beginPositiveReverse(int positivesupernode, int negativesupernode);
	void beginNegative(int positivesupernode, int negativesupernode);
	void beginNegativeReverse(int positivesupernode, int negativesupernode);
};

#endif
