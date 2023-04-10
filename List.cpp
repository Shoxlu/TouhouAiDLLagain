
#include "List.h"
#include "Reseau.h"
#include "Input.h"

void List::append(Value value)
{
	List* list = tail_head;
	if (list != nullptr)
	{
		
		for(int i = 0 ; i < list_length; i++)
		{
			
			if (list->next != nullptr)
			{
				list = list->next;
			}
		}

	}
	list_length++;
	list->next = new List(list, list_length, value, tail_head);
	
}

void List::remove(int index_)
{
	List* list_next = tail_head->next;
	for (List* list=tail_head; list->index < list_length-1; list=list_next)
	{
		if (list == nullptr)
		{
			throw("List index out of range !");
			break;
		}
		if (list->next->index == index_)
		{
			list->next = list;
			list_next = list->next->next;
			index_++;
			break;
		}
	}
}
int List::length()
{
	return list_length;
}
Value List::get(int i)
{
	List* list = tail_head;
	for (int j = 0; j < i-1; j++)
	{
		list = list->next;
	}
	return list->value;
}
List::List() : next(nullptr), previous(nullptr), value(NULL), list_length(0), index(0), tail_head(this)
{

}
List::List(List* previous_, int list_length_, Value value_, List* head) : next(nullptr), previous(previous_), value(value_), list_length(list_length_), index(previous_->index+1), tail_head(head)
{

}

List::~List() {
	if (next != nullptr)
	{
		delete next;
	}
}
