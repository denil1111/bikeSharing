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

// station������
#define STATION_CAPACITY 10
// capacity
#define VEHICLE_CAPACITY 30

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

//template <typename CM>
class BikeSharing
{

private:

	// վ�����
	int _stationNum;
	// TSP�����������
	vector<int> _path;
	// �ܻ���
	double _sum;
	// վ������
	vector<int> _stationDemand;
	// ��ȫͼ
	FullGraph *g;
	// ����Map
	DoubleEdgeMap *cost;
	// վ������
	FullGraph::NodeMap<dim2::Point<double> > *pos;
	// station������
	const int K = STATION_CAPACITY;
	// capacity
	const int Q = VEHICLE_CAPACITY;
	// ��վ����ʱ��,������ʹ����ͷŽ����vector�С�����´����ֵ�����㣬˵���޽�
	vector<int> _positiveStationVisiteFlag;
	// ��ʼվ����
	int _startStation;
	// check
	//TspCheck tspCheck;

public:

	BikeSharing(int num);

	// �������֧�֣������ã���������Ҫ��
	BikeSharing();

	// �������ÿ��վ������������� -STATION_CAPACITY��STATION_CAPACITY ��
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

	// ��ȡ��ʼ�����㣺
	int getStartStation();

	void printResult();

	void runRandom();

	void run();

};// class BikeSharing

#endif
