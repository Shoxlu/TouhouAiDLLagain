#include "List.h"

List::List() {
	head = nullptr;
	len = 0;
}

List::List(int size) {
	head = new ListNode(0);
	ListNode* cur = head;
	for (int i = 0; i < size - 1; i++) {
		cur->next = new ListNode(0);
		if (cur->next)
		{
			cur = cur->next;
		}
	}
	len = size;
}

List::~List() {
	for (int i = 0; i < len; i++) {
		remove(i);
	}
}

void List::append(float value)
{
	if (!head) {
		head = new ListNode(value);
		return;
	}
	for (ListNode* cur = head; cur; cur = cur->next) {
		if (!cur->next) {
			cur->next = new ListNode(value);
			return;
		}
	}
}
void List::remove(int i) {
	if (i >= len) {
		return;
	}
	if (!head) {
		return;
	}
	ListNode* cur = head;
	for (int index = 0 ; index < len; index++) {
		
		if (index == i) {
			if (cur->next) {
				ListNode* temp = cur->next;
				delete cur;
				cur = temp;
				}
			index++;
			break;
		}
		if (cur->next)
		{
			cur = cur->next;
		}
	}
	len -= 1;
}
float List::get(int i) {
	if (i >= len) {
		return -1;
	}
	if (!head) {
		return -1;
	}
	ListNode* cur = head;
	for (int index = 0; index < len; index++) {

		if (index == i) {
			return cur->value;
		}
		if (cur->next)
		{
			cur = cur->next;
		}
	}
	return -1;
}

ListNode::ListNode(float val) {
	value = val;
	next = nullptr;
}