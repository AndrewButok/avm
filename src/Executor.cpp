//
// Created by Andrii BUTOK on 2019-07-23.
//

#include <IOperand.hpp>
#include <iostream>
#include "Executor.hpp"

Executor* Executor::_instance = nullptr;

Executor *Executor::getInstance() {
	if (_instance == nullptr)
		_instance = new Executor();
	return _instance;
}

Executor::Executor() {
	this->_stack = new std::vector<const IOperand *>();
}

Executor::~Executor() {
	delete this->_stack;
}

void Executor::pushToStack(const IOperand *operand) {
	if (operand != nullptr)
		this->_stack->push_back(operand);
}

void Executor::popFromStack() {
	if (this->_stack->empty())
		throw std::out_of_range("Pop error: Stack is empty");
	this->_stack->pop_back();
}

void Executor::assertFromStack(const IOperand *operand) {
	if (operand != nullptr){
		const IOperand *last = *(this->_stack->rbegin());
		if (last->getType() != operand->getType() &&
			last->toString() == operand->toString())
			throw std::invalid_argument("Assert error: Different operand types");
		if (last->getType() != operand->getType() &&
			last->toString() != operand->toString())
			throw std::invalid_argument("Assert error: Different operand types and values");
		if (last->toString() != operand->toString())
			throw std::invalid_argument("Assert error: Different operand values");
	}
}

void Executor::dumpStack() {
	for(auto iterator = this->_stack->rbegin(); iterator != this->_stack->rend();
		iterator++){
		std::cout << *iterator << std::endl;
	}
}
