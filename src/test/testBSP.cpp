
#include "uncapacitatedBSP.h"
#include "capacitated.h"
#include "noZeroCapacitatedBSP.h"
#include "ktimescapacitatedBSP.h"
#include "zeroonlyafterpositivepiece.h"
#include "generalBSP.h"

using namespace   std;
using namespace lemon;

int main() {
	int STATION_NUM = 1;
	int RUNS = 1;
	int VEHICLE_CAPACITY;
	int STATION_CAPACITY;
	clock_t Tstart, Ttsp, Tubsp, Tkbsp, Tcbsp, Tzoapp, Tnzcbsp, Tgbsp;
	double time1 = 0, time2 = 0, time3 = 0, time4 = 0, time5 = 0, time6 = 0;
	double bizhi0 = 0, bizhi1 = 0, bizhi2 = 0, bizhi3 = 0;

	cout << "Input station num:" << endl;
	cin >> STATION_NUM;
	cout << "Input run time:" << endl;
	cin >> RUNS;


	for (int i = 0; i < RUNS; i++){
		TspBase tspbase(STATION_NUM, 10, 20);
		tspbase.randomData();
		//tspbase.inputData();


		Tstart = clock();
		tspbase.data();
		Ttsp = clock();
		time1 = (double)(Ttsp - Tstart);

		UncapacitatedBSP ubsp(tspbase);
		ubsp.run();
		Tubsp = clock();
		time2 += time1 + (double)(Tubsp - Ttsp);
		bizhi0 = ubsp._minSum;

		ZeroOnlyAfterPositivePiece zoapp(tspbase);
		zoapp.run();
		Tzoapp = clock();
		time3 += time1 + (double)(Tzoapp - Tubsp);
		bizhi1 += zoapp._minSum / bizhi0;

		NoZeroCapacitatedBSP nzcbsp(tspbase);
		nzcbsp.run();
		Tnzcbsp = clock();
		time4 += time1 + (double)(Tnzcbsp - Tzoapp);
		bizhi2 += nzcbsp._minSum / bizhi0;

		Tstart = clock();
		tspbase.dataGeneral();
		Ttsp = clock();
		time5 = (double)(Ttsp - Tstart);

		GeneralBSP gbsp(tspbase);
		gbsp.run();
		Tgbsp = clock();
		time6 += time5 + (double)(Tgbsp - Ttsp);
		bizhi3 += gbsp._minSum / bizhi0;

		//KTimesCapacitatedBSP kbsp(tspbase);
		//kbsp.run();

		//CapacitatedBSP cbsp(tspbase);
		//cbsp.run();

		delete tspbase.cost;
		delete tspbase.g;
		delete tspbase.pos;
	}

	cout << "ZeroOnlyAfterPositivePiece:  " << (time3 / RUNS) / CLOCKS_PER_SEC * 1000 << endl;
	cout << "NoZeroCapacitatedBSP:  " << (time4 / RUNS) / CLOCKS_PER_SEC * 1000 << endl;
	cout << "GeneralBSP:  " << (time6 / RUNS) / CLOCKS_PER_SEC * 1000 << endl;
	cout << endl;
	cout << "ZeroOnlyAfterPositivePiece:  " << bizhi1 / RUNS << endl;
	cout << "NoZeroCapacitatedBSP:  " << bizhi2 / RUNS << endl;
	cout << "GeneralBSP:  " << bizhi3 / RUNS << endl;


	system("pause");

	return 0;
}
