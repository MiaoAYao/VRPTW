#include "Problem.h"

#include<math.h>

#define DIV 1
#define DEMAND 200

//��������ʽ�㷨��Ҫ�Ĳ���
/*1 1 1 0
  1 2 1 0
  1 1 0 1
  1 2 0 1*/
#define U1 1
#define U2 1      
#define M1 1
#define M2 0

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
		for (int j = 0; j < i; ++j)
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
		allTime += routeSet[i]->at_depot - routeSet[i]->dt_depot;
	}
}

void Problem::computeLength()
{
	for (int i = 0; i < routeSet.size(); ++i)
	{
		Node* tmp = routeSet[i]->head;
		double m = 1;
		double n = tmp->num;
		double dis_mn;
		while (true)
		{
			if (m > n)
				dis_mn = dis[m - 2][n - 1];
			else
				dis_mn = dis[n - 2][m - 1];

			allLength += dis_mn;

			tmp = tmp->next;
			if (tmp == NULL)
				break;
			else
			{
				m = n;
				n = tmp->num;
			}
		}
		allLength += dis[n - 2][0];
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
			//cout << "***************************************" << endl;
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
	//�жϻص��ֿ��ʱ����û��Υ��ʱ�䴰
	if (p_dTime + dis[p_num - 2][0] > data[0]->l_t)
		return false;

	return true;
}

void Problem::updateRoute(Route* route, double num)
{
	
	double i = num;		//ǰһ���ڵ���
	double dt_i = data[i - 1]->d_t;   //��ǰһ���ڵ��뿪��ʱ��
	double dis_ij;    //�����ڵ�֮��ľ���
	double j;		//��ǰ�ڵ���
	double at_j;
	double wt_j;
	double dt_j;

	Node* tmp = data[num - 1]->next;
	while (tmp)
	{
		j = tmp->num;

		if (i > j)
			dis_ij = dis[i - 2][j - 1];
		else
			dis_ij = dis[j - 2][i - 1];

		at_j = dt_i + dis_ij;
		wt_j = at_j > data[j - 1]->e_t ? 0 : data[j - 1]->e_t - at_j;
		dt_j = at_j + wt_j + data[j - 1]->s_t;

		data[j - 1]->a_t = at_j;
		data[j - 1]->w_t = wt_j;
		data[j - 1]->d_t = dt_j;

		tmp = tmp->next;
		i = j;
		dt_i = dt_j;
	}

	route->at_depot = dt_i + dis[i - 2][0];
}

void Problem::addRoute(Route* r1, Route* r2)
{
	r1->tail->next = r2->head;
	r1->tail = r2->tail;
	r1->size += r2->size;
	r1->demand += r2->demand;
	
	updateRoute(r1, r1->tail->num);
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

		route = new Route(data[i], data[0]->e_t, data[i]->d_t + dis[data[i]->num - 2][0]);
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

void Problem::initInfor()
{
	vector<double>* tmp;
	for (int i = 1; i < data.size(); ++i)
	{
		tmp = new vector<double>();
		tmp->push_back(data[i]->num);
		tmp->push_back(-1);
		tmp->push_back(-1);    
		tmp->push_back(-1);
		infor.push_back(*tmp);
	}
}

bool Problem::judgeInsert(double num, double d_t, Node* node)
{
	
	Node* tmp = node;
	double m = num;  //ǰһ���ڵ�ı��
	double n;              //��ǰ�ڵ�ı��
	double m_dt = d_t;   //�뿪ǰһ���ڵ��ʱ��
	double n_at;           //���ﵱǰ�ڵ��ʱ��
	double n_wt;		//�ڵ�ǰ�ڵ�ȴ���ʱ��
	double n_dt;		//�뿪��ǰ�ڵ��ʱ��
	double dis_mn;  //ǰһ�ڵ㵽��ǰ�ڵ�ľ���
	double pf;
	while (tmp)
	{	
		n = tmp->num;
		if (m > n)
			dis_mn = dis[m - 2][n - 1];
		else
			dis_mn = dis[n - 2][m - 1];

		n_at = m_dt + dis_mn;
		n_wt = n_at > data[n - 1]->e_t ? 0 : data[n - 1]->e_t - n_at;
		n_dt = n_at + n_wt + data[n - 1]->s_t;

		pf = n_at - data[n - 1]->a_t;
		if (n_dt > data[n - 1]->l_t)
		{
			return false;
		}
		else if (pf <= data[n - 1]->w_t)   //֮��Ľڵ㶼�ǿ��е�
		{
			return true;
		}
		else
		{
			m_dt = n_dt;
			m = n;
			tmp = tmp->next;
		}
	}
	//�жϻص��ֿ��ʱ����û�г����������ʱ��
	if (m_dt + dis[m - 2][0] > data[0]->l_t)
		return false;

	return true;
}

void Problem::updateInfor(Route* route)
{
	for (int i = 0; i < infor.size(); ++i)
	{

		double u = infor[i][0];  //Ҫ����Ľڵ�u
		Node* tmp = route->head;
		int m = 1, n = tmp->num; //���ڵ�u���뵽�ڵ�m��n֮��

		double insert_m = -1;    //��õĲ���λ��
		double insert_n = -1;

		double c_1 = -1;		//��Ҫ�����ֵ
		double c_11;
		double c_12;
		
		//����Υ������Լ��ʱ���Ž��п��ǲ���λ�õ�ѡ��
		if (data[u - 1]->d + route->demand < DEMAND)
		{
			//m�뵱ǰ�ڵ�u�ľ��룻m��n�ľ��룻u��n�ľ���
			double dis_mu, dis_mn, dis_un;

			/*at_u������ڵ�u��ʱ��*/
			double at_u;
			double wt_u;
			double dt_u;
			double at_n;

			while (tmp || n == 1)
			{
				if (m > u)
					dis_mu = dis[m - 2][u - 1];
				else
					dis_mu = dis[u - 2][m - 1];

				
				if (m > n)
					dis_mn = dis[m - 2][n - 1];
				else
					dis_mn = dis[n - 2][m - 1];

				if (n > u)
					dis_un = dis[n - 2][u - 1];
				else
					dis_un = dis[u - 2][n - 1];

				at_u = data[m - 1]->d_t + dis_mu;
				/*�ж��Ƿ�Υ���˽ڵ�u��ʱ�䴰Լ��
				���Υ���ˣ���ô����Ȼ�ڵ�ǰλ�ò��붼�������ˣ���ô֮���λ��Ҳ���ó�����*/
				if (at_u + data[u - 1]->s_t > data[u - 1]->l_t)
				{
					break;
				}

				//�����ǰ�ڵ�û��Υ��ʱ�䴰Լ��
				/*�������λ�ò���·��ĩβ*/
				c_11 = dis_mu + dis_un - U1 * dis_mn;

				//at_n:����ڵ�u�󵽴�n��ʱ�䣻wt_u���ڽڵ�u�ĵȴ�ʱ�䣻dt_u���ӽڵ�u�ĳ���ʱ��
				wt_u = at_u > data[u - 1]->e_t ? 0 : data[u - 1]->e_t - at_u;
				dt_u = at_u + wt_u + data[u - 1]->s_t;
				at_n = dt_u + dis_un;
				c_12 = at_n - data[n - 1]->a_t;

				
				//ֻ�в���ڵ�u�󣬿���ʹ·������u֮��Ľڵ㶼���У����ܲ��������λ��
				if (judgeInsert(u, dt_u, tmp))
				{
					double tmp_c_1 = M1 * c_11 + M2 * c_12;
					if (c_1 == -1 || tmp_c_1 < c_1)
					{
						c_1 = tmp_c_1;
						insert_m = m;
						insert_n = n;
					}
				}
				
				if (n == 1)
					break;

				tmp = tmp->next;
				if (tmp)     
				{
					m = n;
					n = tmp->num;
				}
				else     //����ڲֿ�֮ǰ����ڵ�u
				{
					m = n;
					n = 1;
				}
			}	
		}
		
		infor[i][1] = insert_m;
		infor[i][2] = insert_n;
		infor[i][3] = c_1;
	}
}

void Problem::initRoute(int flag)
{
	//ѡ���������ʼ��·���Ľڵ���
	double c_num;
	//infor��Ҫɾ������Ŀ
	int del;

	/*����ѡ��1����ֿ���Զ�Ľڵ㣻2������ʱ�俪ʼ����Ľڵ�*/
	if (flag == 1)
	{
		c_num = infor[0][0];
		double maxDis = dis[c_num - 2][0];
		del = 0;

		//��û������·������ֿ���Զ�Ľڵ�
		for (int i = 1;i < infor.size(); ++i)
		{
			if (dis[infor[i][0] - 2][0] > maxDis)
			{
				c_num = infor[i][0];
				maxDis = dis[c_num - 2][0];
				del = i;
			}
		}
	}
	else if (flag == 2)
	{	
		c_num = infor[0][0];
		double eTime = data[c_num]->e_t;
		del = 0;
		
		
		//��û������·���з���ʱ�俪ʼ����Ľڵ�
		for (int j = 1 ; j < infor.size(); ++j)
		{
			if (data[infor[j][0]]->e_t < eTime)
			{
				c_num = infor[j][0];
				eTime = data[c_num]->e_t;
				del = j;
			}
		}
	}

	//ɾ��infor�е�del����Ŀ
	infor.erase(infor.begin() + del);

	//�Ӳֿ�ڵ�data[0]�ĳ���ʱ��Ϊdata[0]->e_t
	data[c_num - 1]->a_t = data[0]->e_t + dis[c_num - 2][0];
	data[c_num - 1]->w_t = data[c_num - 1]->e_t > data[c_num - 1]->a_t ? data[c_num - 1]->e_t - data[c_num - 1]->a_t : 0;
	data[c_num - 1]->d_t = data[c_num - 1]->a_t + data[c_num - 1]->w_t + data[c_num - 1]->s_t;
	
	Route* route = new Route(data[c_num - 1], data[0]->e_t, data[c_num]->d_t + dis[c_num - 1][0]);
	routeSet.push_back(route);

}

void Problem::useInsertion()
{
	int flag;
	cout << "------- --ѡ���ʼ��һ��·���ķ���----------------------" << endl;
	cout << "----------���ѡ��ʹ����ֿ���Զ�Ľڵ��ʼ��·�������룺1---------------" << endl;
	cout << "----------���ѡ��ʹ�÷���ʱ�俪ʼ����Ľڵ��ʼ��·�������룺2---------" << endl;
	cin >> flag;

	Route* nRoute;
	initInfor();
	while (infor.size() != 0)   //һֱ�����µ�·����ֱ�����еĽڵ㶼���뵽һ��·����
	{
		cout << "����һ���µ�·��..." << endl;
		//����һ���µ�·����ӵ�·������ĩβ
		initRoute(flag);         
		//nRouteΪ��ǰ��·��
		nRoute = routeSet.back();
		

		/*����ܹ��ڵ�ǰ·���п����ҵ����еĲ���*/
		while (true)
		{

			cout << "��ǰ·��Ϊ��";
			nRoute->print();

			//����infor
			updateInfor(nRoute);

			//�ҵ���õ��Ǹ��ڵ���뵽·����
			double c_2;
			double num = -1; //Ҫ����Ľڵ����ĿΪinfor[num]
			int i;
			for (i = 0; i < infor.size(); ++i)
			{
				if (infor[i][1] != -1) //infor[i][1]Ϊ-1ʱ��ʾ�ڵ�ǰ·����û�п��еĲ���ѡ��
				{
					c_2 = U2 * dis[infor[i][0] - 2][0] - infor[i][2];
					num = i;
					break;
				}
			}
			for (; i < infor.size(); ++i)
			{
				if (infor[i][1] != -1)
				{
					double tmp_c_2 = U2 * dis[infor[i][0] - 2][0] - infor[i][3];
					if (tmp_c_2 > c_2)
					{
						c_2 = tmp_c_2;
						num = i;
					}
				}
			}

			
			if (num == -1)  //�ڵ�ǰ·�����޷��ҵ����еĲ���
			{
				break;
			}
			else
			{
				double u;
				double insert_m, insert_n;  //�ڱ��Ϊinsert_m��insert_n�Ľڵ��м����ýڵ�
				//������Ϣ��infor[num]�д洢
				u = infor[num][0];
				insert_m = infor[num][1];
				insert_n = infor[num][2];
				//����Ŀinfor[num]ɾ������ʾ�ڵ�u�Ѿ���ӵ�·����
				infor.erase(infor.begin() + num);

				/*���ڵ�u���뵽insert_m��insert_n֮��*/
				cout << "��" << insert_m << "��" << insert_n << "֮�����ڵ㣺" << u << endl;
				if (insert_m == 1)
				{
					data[u - 1]->next = data[insert_n - 1];
					nRoute->head = data[u - 1];
				}
				else if (insert_n == 1)
				{
					data[insert_m - 1]->next = data[u - 1];
					nRoute->tail = data[insert_m - 1];
				}
				else
				{
					data[insert_m - 1]->next = data[u - 1];
					data[u - 1]->next = data[insert_n - 1];
				}
					

				/*���½ڵ�u�ĵ���ʱ�����Ϣ*/
				double dis_mu;
				if (insert_m > u)
					dis_mu = dis[insert_m - 2][u - 1];
				else
					dis_mu = dis[u - 2][insert_m - 1];

				data[u - 1]->a_t = data[insert_m - 1]->d_t + dis_mu;
				data[u - 1]->w_t = data[u - 1]->a_t > data[u - 1]->e_t ? 0 : data[u - 1]->e_t - data[u - 1]->a_t;
				data[u - 1]->d_t = data[u - 1]->a_t + data[u - 1]->w_t + data[u - 1]->s_t;


				/*���½ڵ�u֮��ڵ�ĵ���ʱ���*/
				updateRoute(nRoute, u);
			}
		}
	}
	

	cout << "------------------------------------------------------" << endl;
	cout << "---------�ɲ�������ʽ�㷨�õ���һ�����н�-------------" << endl;
	cout << "------------------------------------------------------" << endl;
	computeLength();
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


