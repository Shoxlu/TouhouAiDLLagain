#pragma once
class GenericNeurone {
public:
	virtual ~GenericNeurone() {}
	virtual float get_output() = 0;
	virtual float get_weight() = 0;
	virtual void Activation() = 0;
	virtual void mutation_Input() = 0;
};