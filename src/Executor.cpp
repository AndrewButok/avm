//
// Created by Andrii BUTOK on 2019-07-23.
//

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Executor.hpp"

Executor* Executor::_instance = nullptr;

Executor *Executor::getInstance() {
	if (_instance == nullptr)
		_instance = new Executor();
	return _instance;
}

Executor::Executor() {
	this->_stack = new std::vector<const IOperand *>();
	this->_operandFactory = OperandFactory::getInstance();
}

Executor::~Executor() {
	delete this->_stack;
	delete this->_operandFactory;
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

void Executor::add() {
	try {
		this->_checkStack(2);
		auto iter = this->_stack->rbegin();
		const IOperand *rOperand = *iter++;
		const IOperand *lOperand = *iter;
		const IOperand *result;
		if (lOperand->getType() != rOperand->getType())
		{
			if (lOperand->getType() > rOperand->getType()) {
				const IOperand *buf = this->_operandFactory->createOperand(
						lOperand->getType(), rOperand);
				delete rOperand;
				rOperand = buf;
			}
			else {
				const IOperand *buf = this->_operandFactory->createOperand(
						rOperand->getType(), lOperand);
				delete lOperand;
				lOperand = buf;
			}
		}
		result = *lOperand + *rOperand;
		this->_stack->pop_back();
		this->_stack->pop_back();
		this->_stack->push_back(result);
	} catch (std::exception &ex) {
		throw std::runtime_error("Addition error: " + std::string(ex.what()));
	}
}

void Executor::_checkStack(int needed) const {
	if (this->_stack->empty())
		throw std::out_of_range("Stack is empty");
	if (this->_stack->size() < needed) {
		std::stringstream ss("Stack have only ");
		ss << this->_stack->size() << " element(s)";
		throw std::out_of_range(ss.str());
	}
}
