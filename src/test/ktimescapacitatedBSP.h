#ifndef K_TIMES_CAPACITATED_BSP_H
#define K_TIMES_CAPACITATED_BSP_H

#include "bspbase.h"

class KTimesCapacitatedBSP:public BspBase
{
public:

	void run();


public:

	KTimesCapacitatedBSP(TspBase &tspbase);
	~KTimesCapacitatedBSP();

	
	void getSuperNodePieces(int number);
	void calculateMinCostAmongSuperNode();

};

#endif
