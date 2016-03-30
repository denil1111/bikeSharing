/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
* This file is a part of LEMON, a generic C++ optimization library.
*/Users/zyhc/code/project/bike/src/cpp/binding.cc
* Copyright (C) 2003-2013
* Egervary Jeno Kombinatorikus Optimalizalasi Kutatocsoport
* (Egervary Research Group on Combinatorial Optimization, EGRES).
*
* Permission to use, modify and distribute this software is granted
* provided that this copyright notice appears in all copies. For
* precise terms see the accompanying LICENSE file.
*
* This software is provided "AS IS" with no warranty of any kind,
* express or implied, and with no claim as to its suitability for any
* purpose.
*
*/
//#pragma comment(lib,"pthreadVSE2")
//#pragma comment(linker,"/NODEFAULTLIB:libcd /DEFAULTLIB:libcmt")
//#pragma comment(lib,"pthreadVSE2.lib")  
#include <iostream>

#include <lemon/full_graph.h>
#include "lemon/math.h"
#include "lemon/maps.h"
#include "lemon/random.h"
#include "lemon/dim2.h"

#include "lemon/christofides_tsp.h"

//#include "test_tools.h"
#include "bike_sharing.h"

// ’æµ„∏ˆ ˝
#define STATION_NUM 5000

using namespace   std;
using namespace lemon;
using namespace bikesharing;



int main() {

	GRAPH_TYPEDEFS(FullGraph);

	/*œ¬√Ê «Œ“µƒ≤‚ ‘¥˙¬Î
	dim2::Point<double> p1;
	dim2::Point<double> p2;
	cin >> p1;
	cout << "(" << p1.x << "," << p1.y <<")"<< endl;
	*/

	BikeSharing bs(STATION_NUM);

	//bs.run();
	bs.runRandom();

	system("pause");

	return 0;
}
