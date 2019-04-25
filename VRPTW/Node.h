#ifndef NODE
#define NODE

#include<iostream>
using namespace std;

#include<vector>

class Node {
private:
	/*最初拥有的信息*/
	double num;          //Customer NO.
	double x;         //XCOORD.
	double y;         //YCOORD.
	double d;         //DEMAND	
	double e_t;       //EARLIEST TIME
	double l_t;       //LATEST TIME
	double s_t;       //SERVICE TIME

	/*分格后拥有的信息*/
	double box_x;
	double box_y;

	/*将节点加入某个路径后获得的信息*/
	double a_t;       //ARRIVE TIME
	double d_t;       //DEPART TIME
	double w_t;       //WAIT TIME
	Node* next;       //指向路径中下一个节点

public:
	Node(){}
	Node(vector<double>* data);

	friend class Route;
	friend class Problem;
};
#endif