#include "Problem.h"

#include<math.h>

#define DIV 1
#define DEMAND 200

void Problem::buildProblem()
{
	/* �Լ�ѡ���ļ�
	string file_name;
	cout << "Enter the name of the file" << endl;
	getline(cin, file_name);
	ifstream in(file_name); */

	// ʹ���ļ�R101���в���
	ifstream in("R101.txt");

	/* �����ļ��д洢�����ݹ���һ��ͼ */
	vector<double> *cus_data;	//ÿһ���ͻ�������
	Node* tmp;                  //�ÿͻ������ݹ���һ���ڵ�
	while (!in.eof())
	{
		cus_data = new vector<double>();
		double d;
		for (int i = 0; i < 7; ++i)
		{
			in >> d;
			cus_data->push_back(d);
		}
		tmp = new Node(cus_data);
		data.push_back(tmp);
	}
	cout << "Finish reading the data from file..." << endl;
	/*�����еĽڵ���зָ�*/
	buildGrid();
	cout << "Finish building the problem..." << endl;
}

void Problem::buildGrid()
{
	int size;
	size = data.size();
	//max_X ���нڵ��x���������Ǹ�
	//max_Y ���нڵ��y���������Ǹ�
	double max_X = data[0]->x, max_Y = data[0]->y;
	for (int i = 1; i < size; ++i)
	{
		if (data[i]->x > max_X)
			max_X = data[i]->x;
		if (data[i]->y > max_Y)
			max_Y = data[i]->y;
	}

	//�����нڵ�������ڵ�һ�������Σ���ΪWIDTH����ΪHEIGHT
	const double WIDTH = (floor(max_X / 100) + 1) * 100;
	const double HEIGHT = (floor(max_Y / 100) + 1) * 100;
	cout << "width = " << WIDTH << endl;
	cout << "Height = " << HEIGHT << endl;

	//���ֵ�С����Ŀ�step_X�͸�step_X
	const int step_X = WIDTH / DIV;
	const int step_Y = HEIGHT / DIV;

	// �ֿⲻ����һ����֮��
	data[0]->box_x = 0;
	data[0]->box_y = 0;
	for (int i = 1; i < size; ++i)
	{
		int box_X = 0, box_Y = 0;
		while (data[i]->x > box_X)
		{
			box_X += step_X;
		}
		while (data[i]->y > box_Y)
		{
			box_Y += step_X;
		}
		data[i]->box_x = box_X;
		data[i]->box_y = box_Y;
	}
	cout << "Finish building the grid..." << endl;
}

void Problem::computeDis()
{
	int size = data.size();
	vector<double>* tmp;
	for (int i = 1; i < size; ++i)
	{
		tmp = new vector<double>();
		//����i�ͱ�iС�Ľڵ�֮��ľ���
		for (int j = i - 1; j >= 0; --j)
		{
			double distance = sqrt(pow(data[i]->x - data[j]->x, 2) + pow(data[i]->y - data[j]->y, 2));
			tmp->push_back(distance);
		}
		dis.push_back(*tmp);
	}
}

void Problem::computeTime()
{
	for (int i = 0; i < routeSet.size();++i)
	{
		allTime += routeSet[i]->tail->d_t - data[0]->e_t + dis[routeSet[i]->tail->num - 2][0];
	}
}

void Problem::buildSavings()
{
	allLength = 0;   //��ʼ����ܳ���
	vector<double> *saving;
	int size = data.size();
	//�����ʡ��ֵ
	for (int i = 1; i < size - 1; ++i)  
	{
		for (int j = i + 1; j < size; ++j)
		{
			//ֻ����ͬһ������ʱ�ż������
			if (data[i]->box_x == data[j]->box_x && data[i]->box_y == data[j]->box_y)
			{
				saving = new vector<double>();
				saving->push_back(dis[i - 1][0] + dis[j - 1][0] - dis[j - 1][i]);
				saving->push_back(i + 1);
				saving->push_back(j + 1);

				savings.push_back(saving);
				// ��savings���е���
				int m = savings.size();
				if (m > 1)
				{
					int n = m - 1;
					while (n > 0)
					{
						int nn = (n - 1) / 2;
						if ((*savings[n])[0] > (*savings[nn])[0])
						{
							vector<double> *temp;
							temp = savings[n];
							savings[n] = savings[nn];
							savings[nn] = temp;
						}
						n = nn;
					}
				}
			}
		}
	}
	cout << "Finish building the savings..." << endl;
}

void Problem::deleteSavings(int cus_i, int cus_j)
{
	//i��j�����ܵ���0����Ϊ0����ֿ⡣�������������˵��ֻɾ��������ġ���������������ɾ��������ģ���ɾ�������ġ�
	vector<double> * tmp = savings.back();
	savings[0] = tmp;
	savings.pop_back();
	downAdjust(0);

	int size = savings.size();
	for (int i = size - 1; i >= 0; --i)
	{
		vector<double>* temp = savings[i];
		if ((*temp)[1] == cus_i || (*temp)[1] == cus_j || (*temp)[2] == cus_i || (*temp)[2] == cus_j)
		{
			tmp = savings.back();
			savings[i] = tmp;
			savings.pop_back();
			downAdjust(i);
		}
	}
}

void Problem::downAdjust(int i)
{
	int size = savings.size();
	for (int j = 2 * i + 1; j < size;)
	{
		vector<double>* temp = savings[i];
		int maxSav = j;
		if (j + 1 < size)
		{
			if ((*savings[j])[0] < (*savings[j + 1])[0])
				maxSav = j + 1;
		}
		savings[i] = savings[maxSav];
		savings[maxSav] = temp;
		j = 2 * maxSav + 1;
	}
}

//ǰһ���ڵ�ı��;ǰһ���ڵ���뿪ʱ��;�����·��
bool Problem::judge(double pre_num, double pre_dTime, Route* r)
{
	double p_num = pre_num;
	double p_dTime= pre_dTime;
	double aTime;              //���ﵱǰ�ڵ��ʱ��
	double wTime;				//�ڵ�ǰ�ڵ�ȴ���ʱ��
	Node* tmp = r->head;        //��ǰ�ڵ�
	while (tmp)
	{
		double distance;
		if (p_num > tmp->num)
			distance = dis[p_num - 2][tmp->num - 1];
		else
			distance = dis[tmp->num - 2][p_num - 1];
		aTime = p_dTime + distance;
		if (aTime > tmp->l_t)
		{
			return false;
		}
		else
		{
			wTime = tmp->e_t > aTime ? tmp->e_t - aTime : 0;
			p_dTime = aTime + wTime + tmp->s_t;
			p_num = tmp->num;
			tmp = tmp->next;
		}
	}
	return true;
}

void Problem::addRoute(Route* r1, Route* r2)
{
	r1->tail->next = r2->head;
	r1->tail = r2->tail;
	r1->size += r2->size;
	r1->demand += r2->demand;
}

void Problem::useSavings()
{
	Route *route;

	//Build original solution
	for (int i = 1; i < data.size(); ++i)
	{
		//�Ӳֿ�ڵ�data[0]�ĳ���ʱ��Ϊdata[0]->e_t
		data[i]->a_t = data[0]->e_t + dis[i - 1][0];
		data[i]->w_t = data[i]->e_t > data[i]->a_t ? data[i]->e_t - data[i]->a_t : 0;
		data[i]->d_t = data[i]->a_t + data[i]->w_t + data[i]->s_t;
		route = new Route(data[i]);
		routeSet.push_back(route);
	}
	//Compute original length
	allLength = 0;
	for (int i = 0; i < dis.size(); ++i)
	{
		allLength += dis[i][0] * 2;
	}

	//Build better solution
	while (savings.size() != 0)
	{
		cout << "--------------------------------------------" << endl;
		cout <<"Savings����Ŀ��Ϊ�� " << savings.size() << endl;
		/* �����cus_i -> cus_j���ܹ���ʡ������ıߵ���Ϣ*/
		int cus_i = (*savings[0])[1];
		int cus_j = (*savings[0])[2];
		double sav = (*savings[0])[0];

		cout << "���ӽڵ㣺" << cus_i << "��" << cus_j << endl;

		/* �ҵ��������ϵ������ڵ�ֱ����ĸ�·���� */
		/* route_i:�����ڵ�i��·��   route_j:�����ڵ�j��·��
		   flag_i = 0:�ڵ�i��·��ͷ  flag_i = 1:�ڵ�i��·��β*/
		   /* del_i = 0:��ʾ�ϲ�����·���󣬽ڵ�i����·��ͷ��β����֮del_i = 1*/
		int route_i, route_j;
		int flag_i = 0, flag_j = 0;
		int del_i = 0, del_j = 0;

		//�ҵ�·�����а����ͻ�cus_i��cus_j������·��
		int flag = 0;  //���flag==2����ʾ�Ѿ��ҵ�������·��������ѭ��
		for (int i = 0; i < routeSet.size(); ++i)
		{
			Route* temp = routeSet[i];
			if ((temp->head)->num == cus_i)
			{
				route_i = i;
				++flag;
			}
			else if ((temp->tail)->num == cus_i)
			{
				route_i = i;
				flag_i = 1;
				++flag;
			}

			if ((temp->head)->num == cus_j)
			{
				route_j = i;
				++flag;
			}
			else if ((temp->tail)->num == cus_j)
			{
				route_j = i;
				flag_j = 1;
				++flag;
			}

			if (flag == 2)
				break;
		}
		
		if (routeSet[route_i]->size > 1)
		{
			del_i = 1;
			cout << "ɾ����" << cus_i << "��ص�saving" << endl;
		}
		if (routeSet[route_j]->size > 1)
		{
			del_j = 1;
			cout << "ɾ����" << cus_j << "��ص�saving" << endl;
		}
		

		//�������������saving���ı��ϵ������ͻ����Ѿ���һ��·�����ˡ�
		if (route_i == route_j)
		{
			deleteSavings();
			continue;
		}


		/* �ϲ�·�� */
		/* �Ƿ����Ҫ�ϲ�·����Ҫ�����ж�һ�¿����� */
		/* 1. ·���ϵĽڵ�������Ƿ񳬹�������������*/
		/* 2. ·���ϵĽڵ��Ƿ�Υ��ʱ�䴰Լ��*/
		/* ������У���ô����·���ĺϲ�������ɾ����ǰ��ʡ*/
		if (routeSet[route_i]->demand + routeSet[route_j]->demand > DEMAND)
		{
			//·���ϵĽڵ�����󳬹�������������
			deleteSavings();
			continue;
		}


		if (flag_i == 0 && flag_j == 1)
		{
			//�����Υ��ʱ�䴰�Ļ�
			if (judge(routeSet[route_j]->tail->num, routeSet[route_j]->tail->d_t, routeSet[route_i]))
			{
				//��route_i�ϵĽڵ���ӵ�route_j��
				addRoute(routeSet[route_j], routeSet[route_i]);

				//ɾ��·��route_i
				routeSet.erase(routeSet.begin() + route_i);
			}
			else
			{
				deleteSavings();
				continue;
			}
		}
		else if (flag_i == 1 && flag_j == 0)
		{
			//�����Υ��ʱ�䴰�Ļ�
			if (judge(routeSet[route_i]->tail->num, routeSet[route_i]->tail->d_t, routeSet[route_j]))
			{
				//��route_j�ϵĽڵ���ӵ�route_i��
				addRoute(routeSet[route_i], routeSet[route_j]);

				//ɾ��·��route_i
				routeSet.erase(routeSet.begin() + route_j);
			}
			else
			{
				deleteSavings();
				continue;
			}
		}
		else
		{
			if (flag_i == 0 && flag_j == 0)
			{
				//��route_i�ϵĽڵ㷴ת������route_j��ӵ�route_i֮��
				routeSet[route_i]->reverseRoute();
			}
			else if (flag_i == 1 && flag_j == 1)
			{
				//��route_j�ϵĽڵ㷴ת,������ӵ�route_i֮��
				routeSet[route_j]->reverseRoute();
			}

			//�����Υ��ʱ�䴰�Ļ�
			if (judge(routeSet[route_i]->tail->num, routeSet[route_i]->tail->d_t, routeSet[route_j]))
			{
				//��route_j�ϵĽڵ���ӵ�route_i��
				addRoute(routeSet[route_i], routeSet[route_j]);

				//ɾ��·��route_i
				routeSet.erase(routeSet.begin() + route_j);
			}
			else
			{
				//��֮ǰ�����ĸı仹ԭ
				if (flag_i == 0 && flag_j == 0)
				{
					//��route_i�ϵĽڵ㷴ת������route_j��ӵ�route_i֮��
					routeSet[route_i]->reverseRoute();
				}
				else if (flag_i == 1 && flag_j == 1)
				{
					//��route_j�ϵĽڵ㷴ת,������ӵ�route_i֮��
					routeSet[route_j]->reverseRoute();
				}

				deleteSavings();
				continue;
			}
		}

		/* ���㵱ǰ����·���ܳ��� */
		allLength -= sav;
		cout << "allLength = " << allLength << endl;

		/* ɾ��savings���Ѳ���·��ͷ��β�Ľڵ����ڵıߣ�������һ�μӱ�*/
		if (del_i == 1 && del_j == 1)
		{
			deleteSavings(cus_i, cus_j);
		}
		else if (del_i == 1 && del_j == 0)
		{
			deleteSavings(cus_i);
		}
		else if (del_i == 0 && del_j == 1)
		{
			deleteSavings(cus_j);
		}
		else
		{
			deleteSavings();
		}
		
	}
	cout << "------------------------------------------------------" << endl;
	cout << "---------�ɽ�ʡ����ʽ�㷨�õ���һ�����н�-------------" << endl;
	cout << "------------------------------------------------------" << endl;
	computeTime();
	printSolution();
}

void Problem::printSolution()
{
	cout << "allLength = " << allLength << endl;
	cout << "allTime = " << allTime << endl;
	int size = routeSet.size();
	for (int i = 0; i < size; ++i)
	{
		cout << "·��" << i + 1 << "��";
		routeSet[i]->print();
	}
}


