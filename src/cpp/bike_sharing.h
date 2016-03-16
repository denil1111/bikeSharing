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

/// \ingroup tsp
/// \file
/// \brief Christofides algorithm for symmetric TSP


//#include "lemon/christofides_tsp.h"
//#include "test_tools.h"
#include "lemon/core.h"
#include <stdlib.h>
// #include<windows.h>

using namespace lemon;
using namespace std;

// station◊‹»›¡ø
#define STATION_CAPACITY 10
// capacity
#define VEHICLE_CAPACITY 30

typedef FullGraph::EdgeMap<double> DoubleEdgeMap;

namespace bikesharing {

	
	//template <typename CM>
	class BikeSharing
	{

	private:

		// ’æµ„∏ˆ ˝
		int _stationNum;
		// TSPΩ·π˚–Ú¡–»›∆˜
		vector<int> _path;
		// ◊‹ª®∑—
		double _sum;
		// ’æµ„–Ë«Û
		vector<int> _stationDemand;
		// ÕÍ»´Õº
		FullGraph *g;
		// ª®∑—Map
		DoubleEdgeMap *cost;
		// ’æµ„◊¯±Í
		FullGraph::NodeMap<dim2::Point<double> > *pos;
		// station◊‹»›¡ø
		const int K = STATION_CAPACITY;
		// capacity
		const int Q = VEHICLE_CAPACITY;
		// ’˝’æµ„∑√Œ ±Íº«,»Áπ˚∑√Œ π˝£¨æÕ∑≈Ω¯’‚∏ˆvector÷–°£»Áπ˚œ¬¥Œ”÷¬÷µΩ’‚∏ˆµ„£¨Àµ√˜ŒﬁΩ‚
		vector<int> _positiveStationVisiteFlag;
		// ∆ º’æµ„±Í∫≈
		int _startStation;
		// check
		//TspCheck tspCheck;

	public:

		BikeSharing(int num){
			_stationNum = num;
			_startStation = -1;
			g = new FullGraph(num);
			cost = new DoubleEdgeMap(*g);
			pos = new FullGraph::NodeMap<dim2::Point<double> >(*g);
		}

		// ’‚∏ˆªπ≤ª÷ß≥÷£¨≤ªƒ‹”√£¨±®¥Ì£¨ªπ–Ë“™∏ƒ
		BikeSharing(){
			_stationNum = 0;
			g = new FullGraph(0);
			cost = new DoubleEdgeMap(*g);
		}

		// ÀÊª˙…˙≥…√ø∏ˆ’æµ„µƒ–Ë«Û¡ø£∫£® -STATION_CAPACITY£¨STATION_CAPACITY £©
		void getRandomDemand(){
			//TspCheck tspCheck;
			int temp;
			cout << "Station Demand:" << endl;
			srand((unsigned)time(NULL));
			for (int i = 0; i < _stationNum; i++){
				temp = rand() % (STATION_CAPACITY * 2 + 1) - STATION_CAPACITY;
				_stationDemand.push_back(temp);
				printf("%3d ", temp);
				if ((i + 1) % 20 == 0){
					printf("\n");
				}
			}
			cout << endl << endl;
			//tspCheck.checkDemand(_stationDemand);
		}

		// 
		void getRandomPoints(){
			double a, b;
			//srand((unsigned)time(NULL));
			cout << "Point◊¯±Í£∫" << endl;
			for (FullGraph::NodeIt u(*g); u != INVALID; ++u) {
				// NodeMap£¨ «“ª∏ˆmap£¨∆‰÷–Node «key£¨÷µ «Pointµ„°£
				a = rand() % 100;
				b = rand() % 100;
				(*pos)[u] = dim2::Point<double>(a, b);
				cout << "(" << a << ", " << b << ") ";
			}
			cout << endl << endl << endl;
		}

		void getCost(){
		
			int a[5][5] = { { 0, 1, 2, 3, 4 },
							{ 1, 0, 4, 5, 7 },
							{ 2, 4, 0, 6, 8 },
							{ 3, 5, 6, 0, 9 },
							{ 4, 7, 8, 9, 0 } 
			};
			int b[3][3] = { { 0, 1, 2 }, 
							{ 3, 0, 4 }, 
							{ 5, 6, 0 } 
			};
			int c[4][4] = { { 0, 4, 2, 1 },
							{ 4, 0, 1, 5 },
							{ 2, 1, 0, 2 },
							{ 1, 5, 2, 0 } 
			};
			int d[5][5] = { { 1, 200, 100, 1, 1 },
							{ 200, 1, 1, 1, 1 },
							{ 100, 1, 1, 100, 1 },
							{ 1, 1, 100, 1, 100 },
							{ 1, 1, 1, 100, 1 } 
			};
			int e[5][5] = { { 100, 200, 10000, 100, 100 },
							{ 200, 100, 100, 100, 1000 },
							{ 10000, 100, 100, 100, 100 },
							{ 100, 100, 100, 100, 100 },
							{ 1000, 100, 100, 100, 100 } 
			};
			cout << "ªÒ»°Cost:" << endl;
			int i = 0, j = 0;
			for (FullGraph::NodeIt u(*g); u != INVALID; ++u, i++) {
				j = i;
				for (FullGraph::NodeIt v=u; v != INVALID; ++v, j++) {
					if (u == v) {
						//cout << "cost[" << i << "][" << j << "] = " << 0 << " ";
						cout <<  0 << " ";
						continue;
					}
					//cost[g.edge(u, v)] = (pos[u] - pos[v]).normSquare();
					(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = c[i][j];
					//cout << "cost[" << i << "][" << j << "] = " << (*cost)[(*g).edge(u, v)] << " ";
					cout << (*cost)[(*g).edge(u, v)] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}

		void getRandomCost(){
			int i = 0, j = 0;
			cout << "ªÒ»°Cost:" << endl;
			for (FullGraph::NodeIt u(*g); u != INVALID; ++u, i++) {
				for (FullGraph::NodeIt v=u; v != INVALID; ++v, j++) {
					if (u == v) {
						//cout << "cost[" << i << "][" << j << "] = " << 0 << " ";
						//cout << 0 << " ";
						continue;
					}
					(*cost)[(*g).edge(v, u)] = (*cost)[(*g).edge(u, v)] = ( (*pos)[u] - (*pos)[v] ).normSquare();
					//cout << (*cost)[(*g).edge(u, v)] << " ";
				}
				//cout << endl;
				j = 0;
			}
			cout << endl;
		}


		// Get a Random TSP sequence
		// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
		template <typename TSP>
		void getRandomTspSequence(const std::string &alg_name) {
			GRAPH_TYPEDEFS(FullGraph);

			//TspCheck tspCheck;
			TSP alg(*g, *cost);

			// “ÚŒ™ «ÕÍ»´Õº£¨“™…Ë÷√√ø“ªÃı±ﬂµƒcost÷µ,’‚∏ˆcost“≤ «“ª∏ˆmap£¨∆‰÷–gµƒ±ﬂŒ™key°£
			

			_sum = alg.run();
			cout << "sum = " << _sum << endl;

			//tspCheck.check(alg.run() > 0, alg_name + ": Wrong total cost");

			std::vector<Node> vec;
			alg.tourNodes(std::back_inserter(vec));
			cout << "TSPÕ®¬∑£∫" << endl;
			for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
			{
				FullGraph::Node node = *it;
				_path.push_back((*g).index(node));
				cout << " " << (*g).index(node);
			}
			cout << endl;

			// œ¬√Ê∂º «check
			//tspCheck.check(tspCheck.checkTour(*g, vec), alg_name + ": Wrong node sequence");
			//tspCheck.check(tspCheck.checkCost(*g, vec, *cost, alg.tourCost()), alg_name + ": Wrong tour cost");

			SimplePath<FullGraph> path;
			alg.tour(path);
			//tspCheck.check(tspCheck.checkTourPath(*g, path), alg_name + ": Wrong tour");
			//tspCheck.check(tspCheck.checkCost(*g, path, *cost, alg.tourCost()),alg_name + ": Wrong tour cost");

		}

		// Get a TSP sequence
		// getTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");
		template <typename TSP>
		void getTspSequence(const std::string &alg_name) {
			GRAPH_TYPEDEFS(FullGraph);
			
			//TspCheck tspCheck;
			TSP alg(*g, *cost);

			_sum = alg.run();
			cout << "sum = " << _sum << endl;

			//tspCheck.check(alg.run() > 0, alg_name + ": Wrong total cost");
			
			std::vector<Node> vec;
			alg.tourNodes(std::back_inserter(vec));
			cout << "TSPÕ®¬∑£∫" << endl;
			for (vector<Node>::const_iterator it = vec.begin(); it != vec.end(); ++it)
			{
				FullGraph::Node node = *it;
				_path.push_back((*g).index(node));
				cout << " " << (*g).index(node);
			}
			cout << endl;

			// œ¬√Ê∂º «check
			//tspCheck.check(tspCheck.checkTour(*g, vec), alg_name + ": Wrong node sequence");
			//tspCheck.check(tspCheck.checkCost(*g, vec, *cost, alg.tourCost()), alg_name + ": Wrong tour cost");

			SimplePath<FullGraph> path;
			alg.tour(path);
			//tspCheck.check(tspCheck.checkTourPath(*g, path), alg_name + ": Wrong tour");
			//tspCheck.check(tspCheck.checkCost(*g, path, *cost, alg.tourCost()),alg_name + ": Wrong tour cost");

		}

		bool isExistNotVisitedPositiveStation(){

			return true;
		}

		bool isAPositiveStation(int number){
			return _stationDemand[number] >= 0;
		}

		// ªÒ»°∆ º≥ˆ∑¢µ„£∫
		int getStartStation(){

			/*
			// “‘œ¬ «≤‚ ‘¥˙¬Î
			int b[7] = { 3, 2, 5, 4, 0, 1, 6 };
			for (int i = 0; i < 7; i++){
				_path.push_back(b[i]);
			}
			int a[7] = { -1, 2, -4, -3, 2, 5, -1 };
			for (int i = 0; i < 7; i++){
				_stationDemand.push_back(a[i]);
			}
			*/
			_positiveStationVisiteFlag.clear();

			vector<int>::const_iterator it = _path.begin();
			while (isExistNotVisitedPositiveStation()){
				// ’˝’æµ„∑√Œ ±Íº«,»Áπ˚∑√Œ π˝£¨æÕ∑≈Ω¯’‚∏ˆvector÷–°£»Áπ˚œ¬¥Œ”÷¬÷µΩ’‚∏ˆµ„£¨Àµ√˜ŒﬁΩ‚
				vector<int>::iterator result = find(_positiveStationVisiteFlag.begin(), _positiveStationVisiteFlag.end(), *it); 
				if (result != _positiveStationVisiteFlag.end()){
					cout << "ŒﬁΩ‚£¨’æµ„±Í∫≈Œ™£∫" << *it << endl;
					break;
				}

				// ’‚ «“ª¥Œ—∞’“µƒø™ º£¨µ⁄“ª∏ˆµ„µƒ–Ë«Û“ª∂®“™¥Û”⁄µ»”⁄0≤≈––
				if (isAPositiveStation(*it)){
					_positiveStationVisiteFlag.push_back(*it);
					_startStation = *it;
					int tempSum = _stationDemand[*it];
					int tempNum = 1;
					
					while (tempSum >= 0 && tempNum < _stationNum){
						++it;
						++tempNum;
						if (it == _path.end()){
							it = _path.begin();
						}
						tempSum += _stationDemand[*it];
						
					}

					if (tempNum == _stationNum){
						cout << "’“µΩ¡À∆ ºµ„£∫" << _startStation << endl;
						return _startStation;
					}

					// »Áπ˚≥ˆœ÷¡À∫Õ–°”⁄0£¨ƒ«æÕÃ¯π˝∫Û√Êªπ «∏∫–Ë«Ûµƒ’æµ„Ã¯π˝£∫
					if (tempSum < 0){
						++it;
						++tempNum;
						if (it == _path.end()){
							it = _path.begin();
						}
						while (_stationDemand[*it] < 0){
							tempSum += _stationDemand[*it];
							++it;
							if (it == _path.end()){
								it = _path.begin();
							}
						}
					}
				}// if “ª¥Œ—∞’“µƒø™ º
				else {
					++it;
				}
			}// while
			return -1;
		}

		void printResult(){
			int tempSum = 0;
			int i = 0;
			vector<int>::iterator itStartStation = find(_path.begin(), _path.end(), _startStation);
			vector<int>::iterator it = itStartStation;
			cout << endl<< "’˝»∑∑√Œ ¬∑æ∂£∫" << endl;
			for (it; it < _path.end(); it++){
				cout << *it << "(" << _stationDemand[*it] << ") " ;
			}
			for (it = _path.begin(); it < itStartStation; ++it){
				cout << *it << "(" << _stationDemand[*it] << ") " ;
			}
			cout << endl << endl;
		}

		void runRandom(){
			// DWORD start_time = GetTickCount();
			// DWORD end_time;

			getRandomPoints();
			getRandomCost();
			getRandomDemand();

			// end_time = GetTickCount();
			// cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;// ‰≥ˆ‘À–– ±º‰

			getRandomTspSequence<ChristofidesTsp<DoubleEdgeMap > >("Christofides");

			// end_time = GetTickCount();
			// cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;// ‰≥ˆ‘À–– ±º‰

			getStartStation();
			printResult();

			// end_time = GetTickCount();
			// cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;// ‰≥ˆ‘À–– ±º‰
		};

		void run(){
			// DWORD start_time = GetTickCount();
			// DWORD end_time;

			getRandomPoints();
			getCost();
			getRandomDemand();

			// end_time = GetTickCount();
			// cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;// ‰≥ˆ‘À–– ±º‰

			getTspSequence< ChristofidesTsp<DoubleEdgeMap > >("Christofides");

			// end_time = GetTickCount();
			// cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;// ‰≥ˆ‘À–– ±º‰

			getStartStation();
			printResult();

			// end_time = GetTickCount();
			// cout << endl << "The run time is:" << (end_time - start_time) << "ms!" << endl << endl;// ‰≥ˆ‘À–– ±º‰
		};

	};// class BikeSharing
}; // namespace bikesharing

#endif
