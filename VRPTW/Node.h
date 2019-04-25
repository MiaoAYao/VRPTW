#ifndef NODE
#define NODE

#include<iostream>
using namespace std;

#include<vector>

class Node {
private:
	/*���ӵ�е���Ϣ*/
	double num;          //Customer NO.
	double x;         //XCOORD.
	double y;         //YCOORD.
	double d;         //DEMAND	
	double e_t;       //EARLIEST TIME
	double l_t;       //LATEST TIME
	double s_t;       //SERVICE TIME

	/*�ָ��ӵ�е���Ϣ*/
	double box_x;
	double box_y;

	/*���ڵ����ĳ��·�����õ���Ϣ*/
	double a_t;       //ARRIVE TIME
	double d_t;       //DEPART TIME
	double w_t;       //WAIT TIME
	Node* next;       //ָ��·������һ���ڵ�

public:
	Node(){}
	Node(vector<double>* data);

	friend class Route;
	friend class Problem;
};
#endif