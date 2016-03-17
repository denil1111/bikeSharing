/* -*- mode: C++; indent-tabs-mode: nil; -*-
*
* This file is a part of LEMON, a generic C++ optimization library.
*
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

#ifndef BIKE_SHARING_H
#define BIKE_SHARING_H

#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <time.h>

#include <lemon/math.h>
#include <lemon/maps.h>
#include <lemon/full_graph.h>
#include <lemon/dim2.h>
#include <lemon/random.h>
#include <lemon/christofides_tsp.h>

using namespace lemon;
using namespace std;

// station总容量
#define STATION_CAPACITY 10
// capacity
#define VEHICLE_CAPACITY 30

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

//template <typename CM>
class BikeSharing
{

private:

	// 站点个数
	int _stationNum;
	// TSP结果序列容器
	vector<int> _path;
	// 总花费
	double _sum;
	// 站点需求
	vector<int> _stationDemand;
	// 完全图
	FullGraph *g;
	// 花费Map
	DoubleEdgeMap *cost;
	// 站点坐标
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	// station总容量
	const int K = STATION_CAPACITY;
	// capacity
	const int Q = VEHICLE_CAPACITY;
	// 正站点访问标记,如果访问过，就放进这个vector中。如果下次又轮到这个点，说明无解
	vector<int> _positiveStationVisiteFlag;
	// 起始站点标号
	int _startStation;
	// check
	//TspCheck tspCheck;

public:

	BikeSharing(int num);

	// 这个还不支持，不能用，报错，还需要改
	BikeSharing();

	// 随机生成每个站点的需求量：（ -STATION_CAPACITY，STATION_CAPACITY ）
	void getRandomDemand();
	// 
	void getRandomPoints();

	void getCost();

	void getRandomCost();

	// Get a Random TSP sequence
	// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	template <typename TSP>
	void getRandomTspSequence(const std::string &alg_name);

	// Get a TSP sequence
	// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
	template <typename TSP>
	void getTspSequence(const std::string &alg_name);

	bool isExistNotVisitedPositiveStation();

	bool isAPositiveStation(int number);

	// 获取起始出发点：
	int getStartStation();

	void printResult();

	void runRandom();

	void run();

};// class BikeSharing

#endif
