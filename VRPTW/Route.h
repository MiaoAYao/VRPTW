#ifndef ROUTE
#define ROUTE

#include"Node.h"
#include"problem.h"

class Route {
private:
	double dt_depot; //从仓库出发的时间
	double demand;   //路径上的总需求
	double distance; //路径上的总距离
	double time;     //路径的持续时间
	int size;        //路径上节点的数目
	Node* head;      //路径上第一个节点
	Node* tail;      //路径上最后一个节点

public:
	Route(){}
	Route(Node* node);
	void reverseRoute();	//反转路径
	void print();

	friend class Problem;
};
#endif
