#ifndef ROUTE
#define ROUTE

#include"Node.h"
#include"problem.h"

class Route {
private:
	double dt_depot; //�Ӳֿ������ʱ��
	double demand;   //·���ϵ�������
	double distance; //·���ϵ��ܾ���
	double time;     //·���ĳ���ʱ��
	int size;        //·���Ͻڵ����Ŀ
	Node* head;      //·���ϵ�һ���ڵ�
	Node* tail;      //·�������һ���ڵ�

public:
	Route(){}
	Route(Node* node);
	void reverseRoute();	//��ת·��
	void print();

	friend class Problem;
};
#endif
