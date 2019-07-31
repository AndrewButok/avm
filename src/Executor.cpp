/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 12:10:52 by abutok            #+#    #+#             */
/*   Updated: 2019/07/31 15:09:12 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "AVMRuntimeError.hpp"
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
    for (auto operand: *this->_stack){
        delete operand;
    }
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
	delete *(this->_stack->end() - 1);
	this->_stack->pop_back();
}

void Executor::assertFromStack(const IOperand *operand) {
	if (operand != nullptr){
		const IOperand *last = *(this->_stack->rbegin());
        const std::string *s1 = &(last->toString());
        const std::string *s2 = &(operand->toString());
		try {
            if (last->getType() != operand->getType() &&
                *s1 == *s2)
                throw std::invalid_argument("Assert error: Different operand types");
            if (last->getType() != operand->getType() &&
                *s1 != *s2)
                throw std::invalid_argument("Assert error: Different operand types and values");
            if (*s1 != *s2)
                throw std::invalid_argument("Assert error: Different operand values");
            delete s1;
            delete s2;
            delete operand;
        } catch (std::invalid_argument &ex) {
            delete s1;
            delete s2;
		    delete operand;
		    throw AVMRuntimeError(ex.what());
		}
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
		const IOperand *result = nullptr;
        const IOperand *buf = nullptr;
		if ((*(iter + 1))->getType() != (*iter)->getType())
		{
			if ((*(iter + 1))->getType() > (*iter)->getType()) {
				buf = this->_operandFactory->createOperand(
                        (*(iter + 1))->getType(), (*iter));
				delete *iter;
                *iter = buf;
			}
			else {
				buf = this->_operandFactory->createOperand(
                        (*iter)->getType(), *(iter + 1));
				delete *(iter + 1);
                *(iter + 1) = buf;
			}
		}
		Operator oper = (*this->_operators)[static_cast<unsigned int>(type)];
		result = ((*(iter + 1))->*oper)(**iter);
		delete *(iter + 1);
        delete *(iter);
		this->_stack->pop_back();
		this->_stack->pop_back();
		this->_stack->push_back(result);
}

void Executor::add() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Add);
	} catch (std::exception &ex) {
		throw AVMRuntimeError("Addition error: " + std::string(ex.what()));
	}
}

void Executor::sub() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Sub);
	} catch (std::exception &ex) {
		throw AVMRuntimeError("Subtraction error: " + std::string(ex.what()));
	}

}

void Executor::mul() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Mul);
	} catch (std::exception &ex) {
		throw AVMRuntimeError("Multiplication error: " + std::string(ex.what()));
	}
}

void Executor::div() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Div);
	} catch (std::exception &ex) {
		throw AVMRuntimeError("Division error: " + std::string(ex.what()));
	}
}

void Executor::mod() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Mod);
	} catch (std::exception &ex) {
		throw AVMRuntimeError("Modulo error: " + std::string(ex.what()));
	}
}

void Executor::print() {
	const IOperand *last = *(this->_stack->rbegin());
	if (last->getType() != IOperand::eOperandType::Int8)
		throw AVMRuntimeError("Wrong operand eType to print");
	const std::string *val = &(last->toString());
	int ch = boost::lexical_cast<int>(*val);
	delete val;
	std::cout << static_cast<char>(ch);
}
