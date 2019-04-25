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

	bool judge(double pre_num, double pre_dTime, Route* r);   //�жϽ�·��r��ӵ���ǰ·�����Ƿ���п�����
	void addRoute(Route* r1, Route* r2); //��·��r��ӵ���ǰ·������
	
	void buildProblem();	//���ļ��ж����ݣ��������ڵ���Ϣ
	void buildGrid();	//�Խڵ���зָ� 
	void computeDis();	//����ڵ�ͽڵ�֮��ľ���

	void buildSavings();	//�����ʡ
	void deleteSavings(int i = 0, int j = 0);	//ɾ���ߵĶ���Ϊi����j��savingֵ��û�в�����Ĭ��ֻɾ��savings�ĵ�һ��Ԫ��
	void downAdjust(int i);	//�������������Ҫɾ���Ľڵ���������ֵ���ǣ������µ���
	void useSavings();	

	void computeTime();

	void printSolution();

private:
	vector<Node*> data;					//��¼�ڵ����Ϣ
	vector<vector<double>> dis;         //��¼ÿ�����ڵ�֮��ľ���
	vector<vector<double> *> savings;	//��¼��ʡ����Ϣ
	
	double allTime;   //����������ܳ���ʱ��,һ��·��������ʱ�������������·���ľ�����ֵ���
	double allLength;  //�������������·���ϵľ���֮��
	vector<Route*> routeSet;
	
};

#endif
