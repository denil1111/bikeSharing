/*
We construct a nonpreemptive tour that is within a factor 5 of the optimal preemptive tour.
The ratio of the distances traveled by an optimal nonpreemptive tour versus a preemptive tour is bounded by 4.
Transporting a single commodity from a set of suppliers to a set of demand points using a vehicle of limited capacity.
k-delivery TSP(a vehicle with a maximum capacity of k pegs).
The k-delivery TSP is NP-hard, even when k = 1.
The distances between the given points satisfy the triangle inequality.
Assume that we can start the vehicle's tour at any location.
With a fixed starting point, our method still applies with an additive factor of 1 in the approximation factors in the worst case.

Algorithm describe:
(1) First construct a tour of all the given points. 
(2) We treat the vehicle as full when it has k/2 pegs on it, and the remaining capacity is used as a "buffer".
(3) neutral segments, positive segments, negative segments.
(4) We compute a minimum-weight perfect matching between the positive and the negative segments.
	When the vehicle is passing through a positive segment on its tour, it delivers the excess pegs to the negative segment to which the positive segment is matched.
(5) Note that the matching does not include all nodes in the original problem, and therefore it could possibly be arbitrarily expensive.
 
 Algorithm:
 1. Construct a tour T that visits all the points.
 2. Fix a reference point P on the tour T .
 3. Traverse the tour T in some direction starting from P .
 4. Compute the excess function excess(e) for each edge e of T . 
	excess(e) = pegs(e)− slots(e), 
	where pegs(e) is the number of pegs encountered before e is traversed, 
	and slots(e) is defined analogously as the number of slots encountered before e.
 5. For each value of i ∈ [0, k/2) do
	(a) Break the tour into pieces by removing all edges with excess(e) ≡ i mod k/2. 
		Call these edges cut edges. Figure 2.1 shows a sample tour and a plot of the excess function for a counterclockwise traversal of the tour (with k = 6 and i = 0).
	(b) We get p-pieces (positive pieces), n-pieces (negative pieces), and 0-pieces (zero or neutral pieces) as follows.
		 • A p-piece is one where the excess function is x on the cut edge preceding the piece and x + k/2 on the cut edge following the piece;
			for all edges e in the piece, excess(e) ∈ (x, x + k/2).
		 • An n-piece is one where the excess function is x on the cut edge preceding the piece and x − k/2 on the cut edge following the piece;
			for all edges e in the piece, excess(e) ∈ (x − k/2, x).
		 • A 0-piece is one where the excess function is x on the cut edge preceding the piece and x on the cut edge following the piece; 
			for all edges e in the piece, either excess(e) ∈ (x − k/2, x) (such a piece is called decreasing) or excess(e) ∈ (x, x + k/2) (such a piece is called increasing).



*/


//#define DEBUG haha

#ifdef  DEBUG
#define PRINTFDemand			printDemand();
#define PRINTFPoints			printPoints();
//#define PRINTFCost				printCost();
#define PRINTFTSPtour			printTSPtour();
#define PRINTFSuperNode			printSuperNodeInformation();
#define PRINTFFinalPath			printFinalPath();
#define PRINTFReuslt			;
#define PRINTFTspPath			printTspPath();
#else
#define PRINTFDemand			;
#define PRINTFPoints			;
#define PRINTFCost				;
#define PRINTFTSPtour			;
#define PRINTFSuperNode			;
#define PRINTFReuslt			;
#define PRINTFFinalPath			;
#define PRINTFTspPath			printTspPath();
#endif