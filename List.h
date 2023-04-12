#pragma once
#include <iostream>
class Reseau;
class GenericNeurone;
class Input;
class Hidden_neurone;
class List;
class Joueur;
union Value;

class List {
public:

	void append(Value value);
	void remove(int index_);
	int length();
	Value get(int i);
	List();
	List(Value value_);
	~List();

private:
	List(List* previous_, int list_length_, Value value_, List* head, List* tail);
	List* head;
	List* tail;
	List* next;
	List* previous;
	Value value;
	int list_length;
	int index;
};

union Value {
	int i;
	float f;
	double d;
	Reseau* reseau;
	GenericNeurone* neuronegen;
	GenericNeurone** pneuronegen;
	Input* input;
	Hidden_neurone* neurone;
	List* plist;
	List list;
	Joueur* joueur;
	Value(int a) : i(a) {};
	Value(float a) : f(a) {};
	Value(double a) : d(a) {};
	Value(Reseau* a) : reseau(a) {};
	Value(GenericNeurone* a) : neuronegen(a) {};
	Value(GenericNeurone** a) : pneuronegen(a) {};
	Value(Input* a) : input(a) {};
	Value(List* a) : plist(a) {};
	Value(List a) : list(a) {};
	Value(Joueur* a) : joueur(a) {};
	~Value() {};
	operator Reseau* () { return reseau; };
	operator Input* () { return input; };
	operator Hidden_neurone* () { return neurone; };
	operator List* () { return plist; };
	operator List () { return list; };
	operator Joueur* () { return joueur; };
	operator GenericNeurone* () { return neuronegen; };
	operator GenericNeurone** () { return pneuronegen; };

	operator int() { return i; };
	operator float() { return f; };
	operator double() { return d; };
	void append(Value value) {
		List(list).append(value);//actually not safe use, but i shouldn't do shit with that
	}
	Value get(int i) {
		return List(list).get(i);//actually not safe use, but i shouldn't do shit with that
	}
};