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

	bool judge(double pre_num, double pre_dTime, Route* r);   //判断将路径r添加到当前路径后，是否具有可行性
	void addRoute(Route* r1, Route* r2); //将路径r添加到当前路径后面
	
	void buildProblem();	//从文件中读数据，并构建节点信息
	void buildGrid();	//对节点进行分格 
	void computeDis();	//计算节点和节点之间的距离

	void buildSavings();	//计算节省
	void deleteSavings(int i = 0, int j = 0);	//删除边的顶点为i或者j的saving值，没有参数则默认只删除savings的第一个元素
	void downAdjust(int i);	//调整最大树，把要删除的节点用最后面的值覆盖，并向下调整
	void useSavings();	

	void computeTime();

	void printSolution();

private:
	vector<Node*> data;					//记录节点的信息
	vector<vector<double>> dis;         //记录每两个节点之间的距离
	vector<vector<double> *> savings;	//记录节省的信息
	
	double allTime;   //解决方案的总持续时间,一段路径的旅行时间在这里与这段路径的距离数值相等
	double allLength;  //解决方案中所有路径上的距离之和
	vector<Route*> routeSet;
	
};

#endif
