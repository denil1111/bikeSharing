�н϶�仯��

1.����TspBase����󣬹��캯�����£�
TspBase(int stationnum, int stationcapacity, int vehiclecapacity);
��1����TspBase���У�����demand���������£�
vector<int> _allStationDemand;
��2����TspBase���У�����station���꣬�������£����Ժ��ԣ���
vector<point> _point;
����point�Ķ������£�
struct point{
	double a;
	double b;
};
��3����TspBase���У�����cost���������£�
vector<vector<double> > _cost;

2.���������㷨�Ķ��󣬲���Ϊ�����TspBase�����Ȼ�����run������
UncapacitatedBSP ubsp(tspbase);
	
ubsp.run();
	
	

KTimesCapacitatedBSP kbsp(tspbase);
	
kbsp.run();

	

CapacitatedBSP cbsp(tspbase);
	
cbsp.run();	

NoZeroCapacitatedBSP nzcbsp(tspbase);
	
nzcbsp.run();


����ʲô�����뼰ʱ��ϵ�ң�����