#pragma once
class GenericNeurone {
public:
	virtual ~GenericNeurone() {}
	virtual float get_output() = 0;
	virtual void Activation() = 0;
	virtual void mutation_Input() = 0;
	virtual bool isActive() = 0;
};