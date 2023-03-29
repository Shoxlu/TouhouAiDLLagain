#pragma once
#include <iostream>
#include <any>


template <typename T> 
class List {
public:

	void append(T value);
	void remove(int index_);
	int length();
	T get(int i);
	List();
	List(List* previous_, int list_length_, T value_);
	~List();

private:
	List* next;
	List* previous;
	T value;
	int list_length;
	int index;
};