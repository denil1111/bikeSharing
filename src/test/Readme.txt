1.merge类有两个构造函数，分别是：
MergeBSP(int num);
MergeBSP(int num, double x, double y);
其中x,y代表depot的坐标，默认为（0，0）。
也可以调用 void setDepot(double x, double y); 方法来设置depot位置。

2.随机调用顺序：
mbsp.runRandom();
注：之前的 mbsp.randomData(); 被合并到了runRandom()方法里面。
注：随机输入的时候由于最后一个点的demand可能会很大，因此将最后一个点的demand打散随机插入到了之前的站点中，因此会出现实际站点数量大于_stationNum，所以在调用mbsp.runRandom()后重新读取_stationNum!

3.用户输入调用顺序：
（1）输入demand，定义如下：
vector<int> _allStationDemand;
映射关系：
vector<int> _mapPartToAll;
vector<int> _mapAllToPart;
_mapAllToPart[i] == -1 表示i点的demand为0。

（2）输入station坐标，定义如下：
vector<point> _point;
其中point的定义如下：
struct point{
	double a;
	double b;
};

（3）输入cost，定义如下：
vector<vector<double> > _cost;

（4）调用：
mbsp.run();

注意：输入的demand总和一定要为0，不然程序检测到以后就会输出正确结果。
注意：demand和point输入的个数至少要大于等于设置的station num，不然就要有段错误啦。

还有什么问题请及时联系我！！！