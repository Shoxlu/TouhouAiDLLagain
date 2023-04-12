#pragma once
class GenericNeurone {
public:
	virtual ~GenericNeurone() {}
	//k is the connection id
	virtual float get_output(int k, int self_id) = 0;
	virtual bool isActive() = 0;
	virtual void change_weights(int n_inputs) = 0;
	virtual int get_n_outputs() = 0;
	virtual void change_input(GenericNeurone** Inputs, int n_inputs) = 0;
};