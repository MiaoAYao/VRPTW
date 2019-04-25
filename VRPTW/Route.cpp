#include"Route.h"


Route::Route(Node* node)
{
	head = node;
	tail = node;
	size = 1;
	demand = node->d;
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
	while (tmp != NULL)
	{
		cout << tmp->num;
		tmp = tmp->next;
		if (tmp != NULL)
			cout << " -> ";
	}
	cout << endl;
}