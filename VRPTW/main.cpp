#include <iostream>
using namespace std;

#include "Problem.h"

int main()
{
	//Build the problem
	Problem problem;
	problem.buildProblem();	
	problem.computeDis();       //计算并存储距离信息

	//选择使用哪种路径构建算法
	int flag;
	cout << "-----------选择路径构建算法-------------" << endl;
	cout << "-----------如果选择节省启发式算法输入：1------------" << endl;
	cout << "-----------如果选择插入启发式算法输入：2------------" << endl;
	cin >> flag;

	if (flag == 1)
	{
		problem.buildSavings();     //根据距离信息计算节省
		problem.useSavings();
	}
	else if (flag == 2)
	{
		problem.useInsertion();
	}
	
	return 0;
}