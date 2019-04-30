#ifndef ROUTE
#define ROUTE

#include"Node.h"
#include"problem.h"

class Route {
private:
	double dt_depot; //从仓库出发的时间
	double at_depot; //回到仓库的时间
	double demand;   //路径上的总需求
	int size;        //路径上节点的数目
	Node* head;      //路径上第一个节点
	Node* tail;      //路径上最后一个节点

public:
	Route(){}
	Route(Node* node, double dt_depot, double at_depot);
	void reverseRoute();	//反转路径
	void print();

	friend class Problem;
};
#endif
