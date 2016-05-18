有较多变化：

1.创建TspBase类对象，构造函数如下：
TspBase(int stationnum, int stationcapacity, int vehiclecapacity);
（1）在TspBase类中，输入demand，定义如下：
vector<int> _allStationDemand;
（2）在TspBase类中，输入station坐标，定义如下（可以忽略）：
vector<point> _point;
其中point的定义如下：
struct point{
	double a;
	double b;
};
（3）在TspBase类中，输入cost，定义如下：
vector<vector<double> > _cost;

2.创建各种算法的对象，参数为上面的TspBase类对象，然后调用run方法：
UncapacitatedBSP ubsp(tspbase);
	
ubsp.run();
	
	

KTimesCapacitatedBSP kbsp(tspbase);
	
kbsp.run();

	

CapacitatedBSP cbsp(tspbase);
	
cbsp.run();	

NoZeroCapacitatedBSP nzcbsp(tspbase);
	
nzcbsp.run();


还有什么问题请及时联系我！！！