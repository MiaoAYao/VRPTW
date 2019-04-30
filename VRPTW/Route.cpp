#include"Route.h"


Route::Route(Node* node, double dt_depot, double at_depot)
{
	head = node;
	tail = node;
	size = 1;
	demand = node->d;

	this->dt_depot = dt_depot;
	this->at_depot = at_depot;
}

void Route::reverseRoute()
{
	if (head && tail && head != tail)
	{
		Node* tmp = head->next;
		Node* back = head;
		Node* front;
		back->next = NULL;
		while (tmp != NULL)
		{
			front = tmp->next;
			tmp->next = back;
			back = tmp;
			tmp = front;
		}
		tail = head;
		head = back;
	}
}

void Route::print()
{
	Node* tmp = head;
	cout << "1 -> ";
	while (tmp != NULL)
	{
		cout << tmp->num;
		tmp = tmp->next;
		cout << " -> ";
	}
	cout << "1" << endl;
}