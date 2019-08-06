#include <utility>

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 12:10:52 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 14:17:29 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Executor.hpp"

Executor* Executor::_instance = nullptr;

Executor *Executor::getInstance() {
	if (_instance == nullptr)
		_instance = new Executor();
	return _instance;
}

Executor::Executor() {
	this->_alive = true;
	this->_operandFactory = OperandFactory::getInstance();
	this->_operators.push_back(&IOperand::operator+);
	this->_operators.push_back(&IOperand::operator-);
	this->_operators.push_back(&IOperand::operator*);
	this->_operators.push_back(&IOperand::operator/);
	this->_operators.push_back(&IOperand::operator%);
}

Executor::Executor(const Executor &){

}

Executor &Executor::operator=(const Executor &){
	return *this;
}

Executor::~Executor() {
	delete this->_operandFactory;
}

void Executor::_checkStack(unsigned int needed) const {
	if (this->_stack.empty())
		throw AVMRuntimeError("Stack is empty");
	if (this->_stack.size() < needed) {
		auto ss = std::stringstream();
		ss << "Stack have only " << this->_stack.size() << " element(s)";
		throw ExecutorException(ss.str());
	}
}

void Executor::pushToStack(IOperandPtr &operand) {
	if (!_alive)
		return;
	if (operand != nullptr)
		this->_stack.push_back(std::move(operand));
}

void Executor::popFromStack() {
	if (!_alive)
		return;
	try {
		this->_checkStack(1);
		this->_stack.pop_back();
	} catch (AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException(std::string("Pop error: ") + ex.what());
	}
}

void Executor::assertFromStack(IOperandPtr &operandPtr) {
	if (!_alive)
		return;
	try {
		this->_checkStack(1);
		if (operandPtr) {
			auto &last = *(this->_stack.rbegin());
			auto s1 = StrPtr{&(last->toString())},
				s2 = StrPtr{&(operandPtr->toString())};
			auto d1 = boost::lexical_cast<double>(*s1),
				d2 = boost::lexical_cast<double>(*s2);
			try {
				if (last->getType() != operandPtr->getType() &&
					d1 == d2)
					throw ExecutorException("Different operand types");
				if (last->getType() != operandPtr->getType() &&
					d2 != d1)
					throw ExecutorException("Different operand types and values");
				if (d1 != d2)
					throw ExecutorException("Different operand values");
			} catch (AVMRuntimeError &ex) {
				throw ExecutorException(ex.what());
			}
		}
	} catch (AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException(std::string("Assert error: ") + ex.what());
	}
}

void Executor::dumpStack() {
	if (!_alive)
		return;
	for(auto iterator = this->_stack.rbegin(); iterator != this->_stack.rend();
		iterator++){
		auto value = StrPtr{&((*iterator)->toString())};
		std::cout << *value << std::endl;
	}
}

void Executor::_executeArithmeticOperator(eOperatorType type) {
	if (!_alive)
		return;
	this->_checkStack(2);
	auto iter = this->_stack.rbegin();
	if ((*(iter + 1))->getType() != (*iter)->getType())
	{
		if ((*(iter + 1))->getType() > (*iter)->getType()) {
			auto buf = this->_operandFactory->createOperand(
					(*(iter + 1))->getType(), (*iter));
			//delete *iter;
			*iter = std::move(buf);
		}
		else {
			auto buf = this->_operandFactory->createOperand(
					(*iter)->getType(), *(iter + 1));
			//delete *(iter + 1);
			*(iter + 1) = std::move(buf);
		}
	}
	auto oper = (this->_operators)[static_cast<unsigned int>(type)];
	auto result = IOperandPtr{((((iter + 1)->get())->*oper)(**iter))};
	this->_stack.pop_back();
	this->_stack.pop_back();
	this->_stack.push_back(std::move(result));
}

void Executor::add() {
	if (!_alive)
		return;
	try {
		this->_executeArithmeticOperator(eOperatorType::Add);
	} catch (std::exception &ex) {
		this->_alive = false;
		throw ExecutorException("Addition error: " + std::string(ex.what()));
	}
}

void Executor::sub() {
	if (!_alive)
		return;
	try {
		this->_executeArithmeticOperator(eOperatorType::Sub);
	} catch (std::exception &ex) {
		this->_alive = false;
		throw ExecutorException("Subtraction error: " + std::string(ex.what()));
	}

}

void Executor::mul() {
	if (!_alive)
		return;
	try {
		this->_executeArithmeticOperator(eOperatorType::Mul);
	} catch (std::exception &ex) {
		this->_alive = false;
		throw ExecutorException("Multiplication error: " + std::string(ex.what()));
	}
}

void Executor::div() {
	if (!_alive)
		return;
	try {
		this->_executeArithmeticOperator(eOperatorType::Div);
	} catch (std::exception &ex) {
		this->_alive = false;
		throw ExecutorException("Division error: " + std::string(ex.what()));
	}
}

void Executor::mod() {
	if (!_alive)
		return;
	try {
		this->_executeArithmeticOperator(eOperatorType::Mod);
	} catch (std::exception &ex) {
		this->_alive = false;
		throw ExecutorException("Modulo error: " + std::string(ex.what()));
	}
}

void Executor::print() {
	if (!_alive)
		return;
	try {
		this->_checkStack(1);
		auto &last = *(this->_stack.rbegin());
		if (last->getType() != IOperand::eOperandType::Int8)
			throw ExecutorException("Wrong operand eType to print");
		auto val = StrPtr{&(last->toString())};
		auto ch = boost::lexical_cast<int>(*val);
		std::cout << static_cast<char>(ch);
	} catch(AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException("Print error: " + std::string(ex.what()));
	}
}

void Executor::max() {
	if (!_alive)
		return;
	try {
		this->_checkStack(1);
		IOperandPtr *max = nullptr;
		for (auto &op: this->_stack)
			if (max == nullptr || **max < *op)
				max = &op;
		auto maxv = StrPtr{&((*max)->toString())};
		std::cout << *maxv << std::endl;
	} catch (AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException("Maximum error: " + std::string(ex.what()));
	}
}

void Executor::min() {
	if (!_alive)
		return;
	try {
		this->_checkStack(1);
		IOperandPtr *min = nullptr;
		for (auto &op: this->_stack)
			if (min == nullptr || **min > *op)
				min = &op;
		auto minv = StrPtr{&((*min)->toString())};
		std::cout << *minv << std::endl;
	} catch (AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException("Minimum error: " + std::string(ex.what()));
	}
}

void Executor::pow() {
	if (!_alive)
		return;
	try {
		this->_checkStack(2);
		auto iter = this->_stack.rbegin();
		auto rOperand = StrPtr{&(*iter++)->toString()},
			lOperand = StrPtr{&(*iter)->toString()};
		this->_stack.pop_back();
		this->_stack.pop_back();
		auto ss = std::stringstream();
		ss << std::pow(boost::lexical_cast<double>(*lOperand),
					   boost::lexical_cast<double>(*rOperand));
		auto result = this->_operandFactory->createOperand(
				IOperand::eOperandType::Double, ss.str());
		this->_stack.push_back(std::move(result));
	} catch (AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException("Power error: " + std::string(ex.what()));
	}
}

void Executor::reset() {
	this->_alive = true;
	this->_stack.clear();
}

void Executor::sqrt() {
	if (!_alive)
		return;
	try{
		this->_checkStack(1);
		auto iter = this->_stack.rbegin();
		auto operand = StrPtr{&(*iter)->toString()};
		auto val = boost::lexical_cast<double>(*operand);
		if (val < 0)
			throw ExecutorException("Cannot be applied to negative number");
		val = std::sqrt(val);
		auto ss = std::stringstream();
		ss << val;
		this->_stack.pop_back();
		this->_stack.push_back(_operandFactory->createOperand(IOperand::eOperandType::Double, ss.str()));
	} catch (AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException("Square root error: " + std::string(ex.what()));
	}
}

void Executor::log() {
	if (!_alive)
		return;
	try{
		this->_checkStack(1);
		auto iter = this->_stack.rbegin();
		auto operand =  StrPtr{&(*iter)->toString()};
		auto val = boost::lexical_cast<double>(*operand);
		if (val < 0)
			throw ExecutorException("Cannot be applied to negative number");
		val = std::log(val);
		auto ss = std::stringstream();
		ss << val;
		this->_stack.pop_back();
		this->_stack.push_back(_operandFactory->createOperand(IOperand::eOperandType::Double, ss.str()));
	} catch (AVMRuntimeError &ex) {
		this->_alive = false;
		throw ExecutorException("Log error: " + std::string(ex.what()));
	}
}

void Executor::kill(){
	this->_alive = false;
}

Executor::ExecutorException::ExecutorException(std::string message)
		: AVMRuntimeError(std::move(message)) {

}
