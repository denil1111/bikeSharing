#ifndef ZEROONLYAFTERPOSITIVEPIECE_BSP_H
#define ZEROONLYAFTERPOSITIVEPIECE_BSP_H

#include "bspbase.h"

class ZeroOnlyAfterPositivePiece :public BspBase
{
public:

	ZeroOnlyAfterPositivePiece(TspBase &tspbase);
	~ZeroOnlyAfterPositivePiece();

	void run();

};

#endif
