#include <iostream>
#include "uncapacitatedBSP.h"
#include "capacitatedBSP.h"
#include "mergeBSP.h"

using namespace   std;
using namespace lemon;

int main() {
	int STATION_NUM = 1;

	cout << "Input station num:" << endl;
	cin >> STATION_NUM;

	UncapacitatedBSP ubsp(STATION_NUM);
	CapacitatedBSP cbsp(STATION_NUM);
	MergeBSP mbsp(STATION_NUM);

//	ubsp.runRandom();
//	cbsp.runRandom();
	mbsp.runRandom();

	system("pause");

	return 0;
}
