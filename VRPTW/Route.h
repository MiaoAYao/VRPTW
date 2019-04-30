#ifndef ROUTE
#define ROUTE

#include"Node.h"
#include"problem.h"

class Route {
private:
	double dt_depot; //�Ӳֿ������ʱ��
	double at_depot; //�ص��ֿ��ʱ��
	double demand;   //·���ϵ�������
	int size;        //·���Ͻڵ����Ŀ
	Node* head;      //·���ϵ�һ���ڵ�
	Node* tail;      //·�������һ���ڵ�

public:
	Route(){}
	Route(Node* node, double dt_depot, double at_depot);
	void reverseRoute();	//��ת·��
	void print();

	friend class Problem;
};
#endif
