#include <iostream>
using namespace std;

#include "Problem.h"

int main()
{
	//Build the problem
	Problem problem;
	problem.buildProblem();

	/* Print the problem
	problem.printRep();
	problem.printData();  */

	//Solution 1
	problem.computeDis();       //���㲢�洢������Ϣ
	problem.buildSavings();     //���ݾ�����Ϣ�����ʡ
	problem.useSavings();

	return 0;

}