
#include "List.h"
#include "Reseau.h"
#include "Input.h"

template class List<Reseau*>;
template class List<int>;
template class List<Input*>;
template class List<float>;

template <typename T>
void List<T>::append(T value)
{
	List* list = this;
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
	list->next = new List<T>(this, list_length, value);
	
}

template <typename T>
void List<T>::remove(int index_)
{
	for (List* list=this; list->index < list_length-1; list=list->next)
	{
		if (list == nullptr)
		{
			throw("List index out of range !");
			break;
		}
		if (list->next->index == index_)
		{
			list->next->next->previous = list;
			list->next = list->next->next;
			index_++;
		}
	}
}
template <typename T>
int List<T>::length()
{
	return list_length;
}
template <typename T>
T List<T>::get(int i)
{
	List* list = this;
	for (int j = 0; j < i-1; j++)
	{
		list = list->next;
	}
	return list->value;
}
template <typename T>
List<T>::List() : next(nullptr), previous(nullptr), value(NULL), list_length(0), index(0)
{

}
template <typename T>
List<T>::List(List* previous_, int list_length_, T value_) : next(nullptr), previous(previous_), value(value_), list_length(list_length_), index(previous_->index+1)
{

}template <typename T>
List<T>::~List() {
	if (next != nullptr)
	{
		delete next;
	}
}
