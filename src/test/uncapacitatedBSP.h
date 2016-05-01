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
	void run();

};

#endif
