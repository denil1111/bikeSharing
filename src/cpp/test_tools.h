/* -*- mode: C++; indent-tabs-mode: nil; -*-
 *
 * This file is a part of LEMON, a generic C++ optimization library.
 *
 * Copyright (C) 2003-2010
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

#ifndef LEMON_TEST_TEST_TOOLS_H
#define LEMON_TEST_TEST_TOOLS_H

///\ingroup misc
///\file
///\brief Some utilities to write test programs.

#include <iostream>
#include <stdlib.h>
#include "bike_sharing.h"

using namespace   std;
using namespace lemon;
using namespace bikesharing;

///If \c rc is fail, writes an error message and exits.

///If \c rc is fail, writes an error message and exits.
///The error message contains the file name and the line number of the
///source code in a standard from, which makes it possible to go there
///using good source browsers like e.g. \c emacs.
///
///For example
///\code check(0==1,"This is obviously false.");\endcode will
///print something like this (and then exits).
///\verbatim file_name.cc:123: error: This is obviously false. \endverbatim
/*
#define check(rc, msg)                                                  \
  {                                                                     \
    if(!(rc)) {                                                         \
      std::cerr << __FILE__ ":" << __LINE__ << ": error: "              \
                << msg << std::endl;                                    \
      abort();                                                          \
    } else { }                                                          \
  }                                                                     \
  */

class TspCheck{

public:

	TspCheck(){
	}

	void checkDemand(vector<int> &stationDemand){
		int sum = 0;
		for (vector<int>::iterator it = stationDemand.begin(); it != stationDemand.end(); ++it){
			sum += *it;
		}
		if (sum != 0){
			std::cerr << __FILE__ << ":" << __LINE__ << ": error : Demand sum not equal zero" << endl;
		}
	}

	void check(double rc, string msg)                                                  
	{ 
		if (!(rc)) {
			std::cerr << __FILE__ ":" << __LINE__ << ": error: "<< msg << std::endl;                                    
			abort();                                                          
		}
		else {}                                                          
	}                                                                     

	// Checks tour validity
	template <typename Container>
	bool checkTour(const FullGraph &gr, const Container &p) {
		FullGraph::NodeMap<bool> used(gr, false);

		int node_cnt = 0;
		for (typename Container::const_iterator it = p.begin(); it != p.end(); ++it)
		{
			FullGraph::Node node = *it;
			//cout << " " << gr.index(node);

			if (used[node]) return false;
			used[node] = true;
			++node_cnt;
		}
		return (node_cnt == gr.nodeNum());
	}

	// Checks tour validity
	bool checkTourPath(const FullGraph &gr, const Path<FullGraph> &p) {
		FullGraph::NodeMap<bool> used(gr, false);

		if (!checkPath(gr, p)) return false;
		if (gr.nodeNum() <= 1 && p.length() != 0) return false;
		if (gr.nodeNum() > 1 && p.length() != gr.nodeNum()) return false;

		for (int i = 0; i < p.length(); ++i) {
			if (used[gr.target(p.nth(i))]) return false;
			used[gr.target(p.nth(i))] = true;
		}
		return true;
	}

	// Checks tour cost
	template <typename CostMap>
	bool checkCost(const FullGraph &gr, const std::vector<FullGraph::Node> &p,
		const CostMap &cost, typename CostMap::Value total)
	{
		typedef typename CostMap::Value Cost;

		Cost s = 0;
		for (int i = 0; i < int(p.size()) - 1; ++i)
			s += cost[gr.edge(p[i], p[i + 1])];
		if (int(p.size()) >= 2)
			s += cost[gr.edge(p.back(), p.front())];

		return !Tolerance<Cost>().different(s, total);
	}

	// Checks tour cost
	template <typename CostMap>
	bool checkCost(const FullGraph &, const Path<FullGraph> &p,
		const CostMap &cost, typename CostMap::Value total)
	{
		typedef typename CostMap::Value Cost;

		Cost s = 0;
		for (int i = 0; i < p.length(); ++i)
			s += cost[p.nth(i)];

		return !Tolerance<Cost>().different(s, total);
	};

	// Algorithm class for Nearest Insertion
	template <typename CM>
	class NearestInsertionTsp:public InsertionTsp<CM> {
	public:
		NearestInsertionTsp(const FullGraph &gr, const CM &cost)
			: InsertionTsp<CM>(gr, cost) {}
		typename CM::Value run() {
			return InsertionTsp<CM>::run(InsertionTsp<CM>::NEAREST);
		}
	};

	// Algorithm class for Farthest Insertion
	template <typename CM>
	class FarthestInsertionTsp:public InsertionTsp<CM> {
	public:
		FarthestInsertionTsp(const FullGraph &gr, const CM &cost)
			: InsertionTsp<CM>(gr, cost) {}
		typename CM::Value run() {
			return InsertionTsp<CM>::run(InsertionTsp<CM>::FARTHEST);
		}
	};

	// Algorithm class for Cheapest Insertion
	template <typename CM>
	class CheapestInsertionTsp : public InsertionTsp<CM> {
	public:
		CheapestInsertionTsp(const FullGraph &gr, const CM &cost)
			: InsertionTsp<CM>(gr, cost) {}
		typename CM::Value run() {
			return InsertionTsp<CM>::run(InsertionTsp<CM>::CHEAPEST);
		}
	};

	// Algorithm class for Random Insertion
	template <typename CM>
	class RandomInsertionTsp : public InsertionTsp<CM> {
	public:
		RandomInsertionTsp(const FullGraph &gr, const CM &cost)
			: InsertionTsp<CM>(gr, cost) {}
		typename CM::Value run() {
			return InsertionTsp<CM>::run(InsertionTsp<CM>::RANDOM);
		}
	};

};


#endif
