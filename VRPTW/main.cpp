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
	problem.computeDis();       //计算并存储距离信息
	problem.buildSavings();     //根据距离信息计算节省
	problem.useSavings();

	return 0;

}