#pragma once
#include <iostream>
class Reseau;
class GenericNeurone;
class Input;
class Hidden_neurone;
class List;
class Joueur;

union Value {
	int i;
	float f;
	double d;
	Reseau* reseau;
	GenericNeurone* neuronegen;
	Input* input;
	Hidden_neurone* neurone;
	List* list;
	Joueur* joueur;
	Value(int a) : i(a) {};
	Value(float a) : f(a) {};
	Value(double a) : d(a) {};
	Value(Reseau* a) : reseau(a) {};
	Value(GenericNeurone* a) : neuronegen(a) {};
	Value(Input* a) : input(a) {};
	Value(List* a) : list(a) {};
	Value(Joueur* a) : joueur(a) {};
	operator Reseau* () { return reseau; };
	operator Input* () { return input; };
	operator Hidden_neurone* () { return neurone; };
	operator List* () { return list; };
	operator Joueur* () { return joueur; };
	operator GenericNeurone* () { return neuronegen; };
	operator int () { return i; };
	operator float () { return f; };
	operator double () { return d; };
};
class List {
public:

	void append(Value value);
	void remove(int index_);
	int length();
	Value get(int i);
	List();
	List(List* previous_, int list_length_, Value value_, List* tail_head);
	~List();

private:
	List* tail_head;
	List* next;
	List* previous;
	Value value;
	int list_length;
	int index;
};