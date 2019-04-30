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

	void buildProblem();	//���ļ��ж����ݣ��������ڵ���Ϣ
	void buildGrid();	//�Խڵ���зָ� 
	void computeDis();	//����ڵ�ͽڵ�֮��ľ���
	void computeLength();  //��������·�����ܳ���
	
	bool judge(double pre_num, double pre_dTime, Route* r);   //�жϽ�·��r��ӵ���ǰ·�����Ƿ���п�����
	void updateRoute(Route* route, double num); //���½ڵ�ĵ���ʱ�����Ϣ����Ҫ���µ�Ϊ�����Ϊnum�Ľڵ����Ľڵ�
	void addRoute(Route* r1, Route* r2); //��·��r��ӵ���ǰ·������

	/*��ʡ����ʽ�㷨*/
	void buildSavings();	//�����ʡ
	void deleteSavings(int i = 0, int j = 0);	//ɾ���ߵĶ���Ϊi����j��savingֵ��û�в�����Ĭ��ֻɾ��savings�ĵ�һ��Ԫ��
	void downAdjust(int i);	//�������������Ҫɾ���Ľڵ���������ֵ���ǣ������µ���
	void useSavings();	

	/*��������ʽ�㷨*/
	void useInsertion();
	void initRoute(int flag);
	void initInfor();
	void updateInfor(Route* route);
	bool judgeInsert(double num, double d_t, Node* node); //����Ľڵ�ı�ţ�����Ľڵ�ĳ���ʱ�䣻����Ľڵ�ĺ�һ���ڵ�
	

	void computeTime();
	void printSolution();

private:
	vector<Node*> data;					//��¼�ڵ����Ϣ
	vector<vector<double>> dis;         //��¼ÿ�����ڵ�֮��ľ���

	vector<vector<double> *> savings;	//��¼��ʡ����Ϣ

	vector<vector<double>> infor; /*δ����·���Ľڵ��ţ���õĲ���λ�ã�����ýڵ�����ӵĿ���*/
	
	double allTime;   //����������ܳ���ʱ��,һ��·��������ʱ�������������·���ľ�����ֵ���
	double allLength;  //�������������·���ϵľ���֮��
	vector<Route*> routeSet;
	
};

#endif
