#include "Problem.h"

#include<math.h>

#define DIV 1
#define DEMAND 200

//插入启发式算法需要的参数
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
	/* 自己选择文件
	string file_name;
	cout << "Enter the name of the file" << endl;
	getline(cin, file_name);
	ifstream in(file_name); */

	// 使用文件R101进行测试
	ifstream in("R101.txt");

	/* 根据文件中存储的数据构造一个图 */
	vector<double> *cus_data;	//每一个客户的数据
	Node* tmp;                  //用客户的数据构造一个节点
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
	/*对所有的节点进行分格*/
	buildGrid();
	cout << "Finish building the problem..." << endl;
}

void Problem::buildGrid()
{
	int size;
	size = data.size();
	//max_X 所有节点的x坐标最大的那个
	//max_Y 所有节点的y坐标最大的那个
	double max_X = data[0]->x, max_Y = data[0]->y;
	for (int i = 1; i < size; ++i)
	{
		if (data[i]->x > max_X)
			max_X = data[i]->x;
		if (data[i]->y > max_Y)
			max_Y = data[i]->y;
	}

	//将所有节点包括在内的一个长方形，宽为WIDTH，高为HEIGHT
	const double WIDTH = (floor(max_X / 100) + 1) * 100;
	const double HEIGHT = (floor(max_Y / 100) + 1) * 100;
	cout << "width = " << WIDTH << endl;
	cout << "Height = " << HEIGHT << endl;

	//划分的小方格的宽step_X和高step_X
	const int step_X = WIDTH / DIV;
	const int step_Y = HEIGHT / DIV;

	// 仓库不在任一方格之内
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
		//计算i和比i小的节点之间的距离
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
	allLength = 0;   //初始解的总长度
	vector<double> *saving;
	int size = data.size();
	//计算节省的值
	for (int i = 1; i < size - 1; ++i)  
	{
		for (int j = i + 1; j < size; ++j)
		{
			//只有在同一个方格时才计算距离
			if (data[i]->box_x == data[j]->box_x && data[i]->box_y == data[j]->box_y)
			{
				saving = new vector<double>();
				saving->push_back(dis[i - 1][0] + dis[j - 1][0] - dis[j - 1][i]);
				saving->push_back(i + 1);
				saving->push_back(j + 1);

				savings.push_back(saving);
				// 对savings进行调整
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
	//i和j不可能等于0，因为0代表仓库。如果不带参数，说明只删除最上面的。如果带参数，则既删除更上面的，又删除其他的。
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

//前一个节点的编号;前一个节点的离开时间;加入的路径
bool Problem::judge(double pre_num, double pre_dTime, Route* r)
{
	double p_num = pre_num;
	double p_dTime= pre_dTime;
	double aTime;              //到达当前节点的时间
	double wTime;				//在当前节点等待的时间
	Node* tmp = r->head;        //当前节点
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
	//判断回到仓库的时间有没有违背时间窗
	if (p_dTime + dis[p_num - 2][0] > data[0]->l_t)
		return false;

	return true;
}

void Problem::updateRoute(Route* route, double num)
{
	
	double i = num;		//前一个节点编号
	double dt_i = data[i - 1]->d_t;   //从前一个节点离开的时间
	double dis_ij;    //两个节点之间的距离
	double j;		//当前节点编号
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

		//从仓库节点data[0]的出发时间为data[0]->e_t
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
		cout <<"Savings的条目数为： " << savings.size() << endl;
		/* 加入边cus_i -> cus_j后，能够节省最大距离的边的信息*/
		int cus_i = (*savings[0])[1];
		int cus_j = (*savings[0])[2];
		double sav = (*savings[0])[0];

		cout << "连接节点：" << cus_i << "和" << cus_j << endl;

		/* 找到这条边上的两个节点分别在哪个路径上 */
		/* route_i:包含节点i的路径   route_j:包含节点j的路径
		   flag_i = 0:节点i在路径头  flag_i = 1:节点i在路径尾*/
		   /* del_i = 0:表示合并两条路径后，节点i仍在路径头或尾，反之del_i = 1*/
		int route_i, route_j;
		int flag_i = 0, flag_j = 0;
		int del_i = 0, del_j = 0;

		//找到路径集中包含客户cus_i和cus_j的两条路径
		int flag = 0;  //如果flag==2，表示已经找到这两条路径，跳出循环
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
			cout << "删除与" << cus_i << "相关的saving" << endl;
		}
		if (routeSet[route_j]->size > 1)
		{
			del_j = 1;
			cout << "删除与" << cus_j << "相关的saving" << endl;
		}
		

		//存在特殊情况，saving最大的边上的两个客户，已经在一条路径上了。
		if (route_i == route_j)
		{
			deleteSavings();
			continue;
		}


		/* 合并路径 */
		/* 是否真的要合并路径，要首先判断一下可行性 */
		/* 1. 路径上的节点的需求是否超过车辆的总容量*/
		/* 2. 路径上的节点是否违背时间窗约束*/
		/* 如果可行，那么进行路径的合并，否则，删除当前节省*/
		if (routeSet[route_i]->demand + routeSet[route_j]->demand > DEMAND)
		{
			//路径上的节点的需求超过车辆的总容量
			deleteSavings();
			continue;
		}


		if (flag_i == 0 && flag_j == 1)
		{
			//如果不违背时间窗的话
			if (judge(routeSet[route_j]->tail->num, routeSet[route_j]->tail->d_t, routeSet[route_i]))
			{
				//将route_i上的节点添加到route_j上
				addRoute(routeSet[route_j], routeSet[route_i]);

				//删除路径route_i
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
			//如果不违背时间窗的话
			if (judge(routeSet[route_i]->tail->num, routeSet[route_i]->tail->d_t, routeSet[route_j]))
			{
				//将route_j上的节点添加到route_i上
				addRoute(routeSet[route_i], routeSet[route_j]);

				//删除路径route_i
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
				//将route_i上的节点反转，并把route_j添加到route_i之后
				routeSet[route_i]->reverseRoute();
			}
			else if (flag_i == 1 && flag_j == 1)
			{
				//将route_j上的节点反转,并且添加到route_i之后
				routeSet[route_j]->reverseRoute();
			}

			//如果不违背时间窗的话
			if (judge(routeSet[route_i]->tail->num, routeSet[route_i]->tail->d_t, routeSet[route_j]))
			{
				//将route_j上的节点添加到route_i上
				addRoute(routeSet[route_i], routeSet[route_j]);

				//删除路径route_i
				routeSet.erase(routeSet.begin() + route_j);
			}
			else
			{
				//将之前做出的改变还原
				if (flag_i == 0 && flag_j == 0)
				{
					//将route_i上的节点反转，并把route_j添加到route_i之后
					routeSet[route_i]->reverseRoute();
				}
				else if (flag_i == 1 && flag_j == 1)
				{
					//将route_j上的节点反转,并且添加到route_i之后
					routeSet[route_j]->reverseRoute();
				}

				deleteSavings();
				continue;
			}
		}

		/* 计算当前所有路径总长度 */
		allLength -= sav;
		cout << "allLength = " << allLength << endl;

		/* 删除savings中已不在路径头或尾的节点所在的边，进行下一次加边*/
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
	cout << "---------由节省启发式算法得到的一个可行解-------------" << endl;
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
	double m = num;  //前一个节点的编号
	double n;              //当前节点的编号
	double m_dt = d_t;   //离开前一个节点的时间
	double n_at;           //到达当前节点的时间
	double n_wt;		//在当前节点等待的时间
	double n_dt;		//离开当前节点的时间
	double dis_mn;  //前一节点到当前节点的距离
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
		else if (pf <= data[n - 1]->w_t)   //之后的节点都是可行的
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
	//判断回到仓库的时间有没有超过最晚服务时间
	if (m_dt + dis[m - 2][0] > data[0]->l_t)
		return false;

	return true;
}

void Problem::updateInfor(Route* route)
{
	for (int i = 0; i < infor.size(); ++i)
	{

		double u = infor[i][0];  //要插入的节点u
		Node* tmp = route->head;
		int m = 1, n = tmp->num; //将节点u插入到节点m和n之间

		double insert_m = -1;    //最好的插入位置
		double insert_n = -1;

		double c_1 = -1;		//需要计算的值
		double c_11;
		double c_12;
		
		//当不违背容量约束时，才进行考虑插入位置的选择
		if (data[u - 1]->d + route->demand < DEMAND)
		{
			//m与当前节点u的距离；m和n的距离；u和n的距离
			double dis_mu, dis_mn, dis_un;

			/*at_u：到达节点u的时间*/
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
				/*判断是否违背了节点u的时间窗约束
				如果违背了，那么，既然在当前位置插入都不可行了，那么之后的位置也不用尝试了*/
				if (at_u + data[u - 1]->s_t > data[u - 1]->l_t)
				{
					break;
				}

				//如果当前节点没有违背时间窗约束
				/*如果插入位置不是路径末尾*/
				c_11 = dis_mu + dis_un - U1 * dis_mn;

				//at_n:插入节点u后到达n的时间；wt_u：在节点u的等待时间；dt_u：从节点u的出发时间
				wt_u = at_u > data[u - 1]->e_t ? 0 : data[u - 1]->e_t - at_u;
				dt_u = at_u + wt_u + data[u - 1]->s_t;
				at_n = dt_u + dis_un;
				c_12 = at_n - data[n - 1]->a_t;

				
				//只有插入节点u后，可以使路径上在u之后的节点都可行，才能插入在这个位置
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
				else     //如果在仓库之前插入节点u
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
	//选择的用来初始化路径的节点编号
	double c_num;
	//infor中要删除的条目
	int del;

	/*两种选择：1，离仓库最远的节点；2，服务时间开始最早的节点*/
	if (flag == 1)
	{
		c_num = infor[0][0];
		double maxDis = dis[c_num - 2][0];
		del = 0;

		//找没被加入路径中离仓库最远的节点
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
		
		
		//找没被加入路径中服务时间开始最早的节点
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

	//删除infor中第del个条目
	infor.erase(infor.begin() + del);

	//从仓库节点data[0]的出发时间为data[0]->e_t
	data[c_num - 1]->a_t = data[0]->e_t + dis[c_num - 2][0];
	data[c_num - 1]->w_t = data[c_num - 1]->e_t > data[c_num - 1]->a_t ? data[c_num - 1]->e_t - data[c_num - 1]->a_t : 0;
	data[c_num - 1]->d_t = data[c_num - 1]->a_t + data[c_num - 1]->w_t + data[c_num - 1]->s_t;
	
	Route* route = new Route(data[c_num - 1], data[0]->e_t, data[c_num]->d_t + dis[c_num - 1][0]);
	routeSet.push_back(route);

}

void Problem::useInsertion()
{
	int flag;
	cout << "------- --选择初始化一条路径的方法----------------------" << endl;
	cout << "----------如果选择使用离仓库最远的节点初始化路径，输入：1---------------" << endl;
	cout << "----------如果选择使用服务时间开始最早的节点初始化路径，输入：2---------" << endl;
	cin >> flag;

	Route* nRoute;
	initInfor();
	while (infor.size() != 0)   //一直产生新的路径，直到所有的节点都加入到一个路径中
	{
		cout << "产生一条新的路径..." << endl;
		//产生一条新的路径添加到路径集的末尾
		initRoute(flag);         
		//nRoute为当前的路径
		nRoute = routeSet.back();
		

		/*如果能够在当前路径中可以找到可行的插入*/
		while (true)
		{

			cout << "当前路径为：";
			nRoute->print();

			//更新infor
			updateInfor(nRoute);

			//找到最好的那个节点插入到路径中
			double c_2;
			double num = -1; //要插入的节点的条目为infor[num]
			int i;
			for (i = 0; i < infor.size(); ++i)
			{
				if (infor[i][1] != -1) //infor[i][1]为-1时表示在当前路径中没有可行的插入选择
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

			
			if (num == -1)  //在当前路径中无法找到可行的插入
			{
				break;
			}
			else
			{
				double u;
				double insert_m, insert_n;  //在编号为insert_m和insert_n的节点中间插入该节点
				//插入信息在infor[num]中存储
				u = infor[num][0];
				insert_m = infor[num][1];
				insert_n = infor[num][2];
				//将条目infor[num]删除，表示节点u已经添加到路径中
				infor.erase(infor.begin() + num);

				/*将节点u插入到insert_m和insert_n之间*/
				cout << "在" << insert_m << "和" << insert_n << "之间插入节点：" << u << endl;
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
					

				/*更新节点u的到达时间等信息*/
				double dis_mu;
				if (insert_m > u)
					dis_mu = dis[insert_m - 2][u - 1];
				else
					dis_mu = dis[u - 2][insert_m - 1];

				data[u - 1]->a_t = data[insert_m - 1]->d_t + dis_mu;
				data[u - 1]->w_t = data[u - 1]->a_t > data[u - 1]->e_t ? 0 : data[u - 1]->e_t - data[u - 1]->a_t;
				data[u - 1]->d_t = data[u - 1]->a_t + data[u - 1]->w_t + data[u - 1]->s_t;


				/*更新节点u之后节点的到达时间等*/
				updateRoute(nRoute, u);
			}
		}
	}
	

	cout << "------------------------------------------------------" << endl;
	cout << "---------由插入启发式算法得到的一个可行解-------------" << endl;
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
		cout << "路径" << i + 1 << "：";
		routeSet[i]->print();
	}
}


