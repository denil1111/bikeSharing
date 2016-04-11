1.merge类有两个构造函数，分别是：
MergeBSP(int num);
MergeBSP(int num, int x, int y);
其中x,y代表depot的坐标，默认为（0，0）。
也可以调用 void setDepot(int x, int y); 方法来设置depot位置。

2.随机调用顺序：
mbsp.runRandom();
注：之前的 mbsp.randomData();
被合并到了run()方法里面。

3.用户输入调用顺序：
（1）输入demand，定义如下：
vector<int> _stationDemand;
（2）输入station坐标，定义如下：
vector<point> _point;
其中point的定义如下：
struct point{
	int a;
	int b;
};
（3）输入cost，定义如下：
vector<vector<int> > _cost;
（4）调用：
mbsp.run();

注意：输入的demand总和一定要为0，不然程序检测到以后就会输出正确结果。
注意：demand和point输入的个数至少要大于等于设置的station num，不然就要有段错误啦。

还有什么问题请及时联系我！！！