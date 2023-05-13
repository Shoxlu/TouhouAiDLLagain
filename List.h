#pragma once
class ListNode {
public:
	ListNode(float val);
	float value;
	ListNode* next;
private:
};
class List {
public:
	List();
	List(int size);
	~List();
	void append(float value);
	void remove(int i);
	float get(int i); //i think it is pretty slow when used a lot of times
	int len;
	ListNode* head;
private:
};

