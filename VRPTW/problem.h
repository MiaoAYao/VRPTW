#ifndef PROBLEM
#define PROBLEM

#include<fstream>
#include<iostream>
using namespace std;

#include"Node.h"
#include"Route.h"

#include<vector>


class Problem {
public:
	Problem() {};

	void buildProblem();	//从文件中读数据，并构建节点信息
	void buildGrid();	//对节点进行分格 
	void computeDis();	//计算节点和节点之间的距离
	void computeLength();  //计算所有路径的总长度
	
	bool judge(double pre_num, double pre_dTime, Route* r);   //判断将路径r添加到当前路径后，是否具有可行性
	void updateRoute(Route* route, double num); //更新节点的到达时间等信息，需要更新的为：编号为num的节点后面的节点
	void addRoute(Route* r1, Route* r2); //将路径r添加到当前路径后面

	/*节省启发式算法*/
	void buildSavings();	//计算节省
	void deleteSavings(int i = 0, int j = 0);	//删除边的顶点为i或者j的saving值，没有参数则默认只删除savings的第一个元素
	void downAdjust(int i);	//调整最大树，把要删除的节点用最后面的值覆盖，并向下调整
	void useSavings();	

	/*插入启发式算法*/
	void useInsertion();
	void initRoute(int flag);
	void initInfor();
	void updateInfor(Route* route);
	bool judgeInsert(double num, double d_t, Node* node); //插入的节点的编号；插入的节点的出发时间；插入的节点的后一个节点
	

	void computeTime();
	void printSolution();

private:
	vector<Node*> data;					//记录节点的信息
	vector<vector<double>> dis;         //记录每两个节点之间的距离

	vector<vector<double> *> savings;	//记录节省的信息

	vector<vector<double>> infor; /*未加入路径的节点编号；最好的插入位置；插入该节点后增加的开销*/
	
	double allTime;   //解决方案的总持续时间,一段路径的旅行时间在这里与这段路径的距离数值相等
	double allLength;  //解决方案中所有路径上的距离之和
	vector<Route*> routeSet;
	
};

#endif
