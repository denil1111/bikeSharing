#ifndef K_TIMES_CAPACITATED_BSP_H
#define K_TIMES_CAPACITATED_BSP_H

#include "bspbase.h"

class KTimesCapacitatedBSP:public BspBase
{
public:

	void run();
	void runRandom();

public:

	KTimesCapacitatedBSP(TspBase &tspbase);
	~KTimesCapacitatedBSP();

	template <typename TSP>
	void getTspTour(const std::string &alg_name);
	void getSuperNodePieces(int number);
	

};

#endif
