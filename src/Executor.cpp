/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 12:10:52 by abutok            #+#    #+#             */
/*   Updated: 2019/07/30 19:23:38 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	this->_operators = new std::vector<Operator>();
	this->_operators->push_back(&IOperand::operator+);
	this->_operators->push_back(&IOperand::operator-);
	this->_operators->push_back(&IOperand::operator*);
	this->_operators->push_back(&IOperand::operator/);
	this->_operators->push_back(&IOperand::operator%);
}

Executor::~Executor() {
	delete this->_stack;
	delete this->_operandFactory;
	delete this->_operators;
}

void Executor::_checkStack(unsigned int needed) const {
	if (this->_stack->empty())
		throw std::out_of_range("Stack is empty");
	if (this->_stack->size() < needed) {
		std::stringstream ss("Stack have only ");
		ss << this->_stack->size() << " element(s)";
		throw std::out_of_range(ss.str());
	}
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
		auto *value = &((*iterator)->toString());
		std::cout << *value << std::endl;
		delete (value);
	}
}

void Executor::_executeArithmeticOperator(eOperatorType type) {

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
		Operator oper = (*this->_operators)[static_cast<unsigned int>(type)];
		result = (lOperand->*oper)(*rOperand);
		this->_stack->pop_back();
		this->_stack->pop_back();
		this->_stack->push_back(result);
}

void Executor::add() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Add);
	} catch (std::exception &ex) {
		throw std::runtime_error("Addition error: " + std::string(ex.what()));
	}
}

void Executor::sub() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Sub);
	} catch (std::exception &ex) {
		throw std::runtime_error("Subtraction error: " + std::string(ex.what()));
	}

}

void Executor::mul() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Mul);
	} catch (std::exception &ex) {
		throw std::runtime_error("Multiplication error: " + std::string(ex.what()));
	}
}

void Executor::div() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Div);
	} catch (std::exception &ex) {
		throw std::runtime_error("Division error: " + std::string(ex.what()));
	}
}

void Executor::mod() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Mod);
	} catch (std::exception &ex) {
		throw std::runtime_error("Modulo error: " + std::string(ex.what()));
	}
}

void Executor::print() {
	const IOperand *last = *(this->_stack->rbegin());
	if (last->getType() != IOperand::eOperandType::Int8)
		throw std::runtime_error("Wrong operand eType to print");
	const std::string *val = &(last->toString());
	int ch = boost::lexical_cast<int>(*val);
	delete val;
	std::cout << static_cast<char>(ch);
}
