1.merge�����������캯�����ֱ��ǣ�
MergeBSP(int num);
MergeBSP(int num, double x, double y);
����x,y����depot�����꣬Ĭ��Ϊ��0��0����
Ҳ���Ե��� void setDepot(double x, double y); ����������depotλ�á�

2.�������˳��
mbsp.runRandom();
ע��֮ǰ�� mbsp.randomData(); ���ϲ�����runRandom()�������档
ע����������ʱ���������һ�����demand���ܻ�ܴ���˽����һ�����demand��ɢ������뵽��֮ǰ��վ���У���˻����ʵ��վ����������_stationNum�������ڵ���mbsp.runRandom()�����¶�ȡ_stationNum!

3.�û��������˳��
��1������demand���������£�
vector<int> _allStationDemand;
ӳ���ϵ��
vector<int> _mapPartToAll;
vector<int> _mapAllToPart;
_mapAllToPart[i] == -1 ��ʾi���demandΪ0��

��2������station���꣬�������£�
vector<point> _point;
����point�Ķ������£�
struct point{
	double a;
	double b;
};

��3������cost���������£�
vector<vector<double> > _cost;

��4�����ã�
mbsp.run();

ע�⣺�����demand�ܺ�һ��ҪΪ0����Ȼ�����⵽�Ժ�ͻ������ȷ�����
ע�⣺demand��point����ĸ�������Ҫ���ڵ������õ�station num����Ȼ��Ҫ�жδ�������

����ʲô�����뼰ʱ��ϵ�ң�����