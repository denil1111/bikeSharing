1.merge�����������캯�����ֱ��ǣ�
MergeBSP(int num);
MergeBSP(int num, int x, int y);
����x,y����depot�����꣬Ĭ��Ϊ��0��0����
Ҳ���Ե��� void setDepot(int x, int y); ����������depotλ�á�

2.�������˳��
mbsp.runRandom();
ע��֮ǰ�� mbsp.randomData();
���ϲ�����run()�������档

3.�û��������˳��
��1������demand���������£�
vector<int> _stationDemand;
��2������station���꣬�������£�
vector<point> _point;
����point�Ķ������£�
struct point{
	int a;
	int b;
};
��3������cost���������£�
vector<vector<int> > _cost;
��4�����ã�
mbsp.run();

ע�⣺�����demand�ܺ�һ��ҪΪ0����Ȼ�����⵽�Ժ�ͻ������ȷ�����
ע�⣺demand��point����ĸ�������Ҫ���ڵ������õ�station num����Ȼ��Ҫ�жδ�������

����ʲô�����뼰ʱ��ϵ�ң�����