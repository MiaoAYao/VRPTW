#include <iostream>
using namespace std;

#include "Problem.h"

int main()
{
	//Build the problem
	Problem problem;
	problem.buildProblem();	
	problem.computeDis();       //���㲢�洢������Ϣ

	//ѡ��ʹ������·�������㷨
	int flag;
	cout << "-----------ѡ��·�������㷨-------------" << endl;
	cout << "-----------���ѡ���ʡ����ʽ�㷨���룺1------------" << endl;
	cout << "-----------���ѡ���������ʽ�㷨���룺2------------" << endl;
	cin >> flag;

	if (flag == 1)
	{
		problem.buildSavings();     //���ݾ�����Ϣ�����ʡ
		problem.useSavings();
	}
	else if (flag == 2)
	{
		problem.useInsertion();
	}
	
	return 0;
}