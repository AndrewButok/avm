#include <utility>

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 12:10:52 by abutok            #+#    #+#             */
/*   Updated: 2019/08/04 14:04:58 by abutok           ###   ########.fr       */
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
		throw AVMRuntimeError("Stack is empty");
	if (this->_stack->size() < needed) {
		auto ss = std::stringstream();
		ss << "Stack have only " << this->_stack->size() << " element(s)";
		throw ExecutorException(ss.str());
	}
}

void Executor::pushToStack(const IOperand *operand) {
	if (operand != nullptr)
		this->_stack->push_back(operand);
}

void Executor::popFromStack() {
	try {
		this->_checkStack(1);
		delete *(this->_stack->end() - 1);
		this->_stack->pop_back();
	} catch (AVMRuntimeError &ex) {
		throw ExecutorException(std::string("Pop error: ") + ex.what());
	}
}

void Executor::assertFromStack(const IOperand *operand) {
	try {
		this->_checkStack(1);
		if (operand != nullptr) {
			auto last = *(this->_stack->rbegin());
			auto s1 = &(last->toString()),
				s2 = &(operand->toString());
			auto d1 = boost::lexical_cast<double>(*s1),
				d2 = boost::lexical_cast<double>(*s2);
			try {
				if (last->getType() != operand->getType() &&
					d1 == d2)
					throw ExecutorException("Different operand types");
				if (last->getType() != operand->getType() &&
					d2 != d1)
					throw ExecutorException("Different operand types and values");
				if (d1 != d2)
					throw ExecutorException("Different operand values");
				delete s1;
				delete s2;
				delete operand;
			} catch (AVMRuntimeError &ex) {
				delete s1;
				delete s2;
				delete operand;
				throw ExecutorException(ex.what());
			}
		}
	} catch (AVMRuntimeError &ex) {
		throw ExecutorException(std::string("Assert error: ") + ex.what());
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
		auto oper = (*this->_operators)[static_cast<unsigned int>(type)];
		result = (((*(iter + 1))->*oper)(**iter));
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
		throw ExecutorException("Addition error: " + std::string(ex.what()));
	}
}

void Executor::sub() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Sub);
	} catch (std::exception &ex) {
		throw ExecutorException("Subtraction error: " + std::string(ex.what()));
	}

}

void Executor::mul() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Mul);
	} catch (std::exception &ex) {
		throw ExecutorException("Multiplication error: " + std::string(ex.what()));
	}
}

void Executor::div() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Div);
	} catch (std::exception &ex) {
		throw ExecutorException("Division error: " + std::string(ex.what()));
	}
}

void Executor::mod() {
	try {
		this->_executeArithmeticOperator(eOperatorType::Mod);
	} catch (std::exception &ex) {
		throw ExecutorException("Modulo error: " + std::string(ex.what()));
	}
}

void Executor::print() {
	try {
		this->_checkStack(1);
		auto last = *(this->_stack->rbegin());
		if (last->getType() != IOperand::eOperandType::Int8)
			throw ExecutorException("Wrong operand eType to print");
		auto val = &(last->toString());
		auto ch = boost::lexical_cast<int>(*val);
		delete val;
		std::cout << static_cast<char>(ch);
	} catch(AVMRuntimeError &ex) {
		throw ExecutorException("Print error: " + std::string(ex.what()));
	}
}

void Executor::max() {
	try {
		this->_checkStack(1);
		const IOperand *max = nullptr;
		for (auto op: *(this->_stack))
			if (max == nullptr || *max < *op)
				max = op;
		auto maxv = &(max->toString());
		std::cout << *maxv << std::endl;
		delete maxv;
	} catch (AVMRuntimeError &ex) {
		throw ExecutorException("Maximum error: " + std::string(ex.what()));
	}
}

void Executor::min() {
	try {
		this->_checkStack(1);
		const IOperand *min = nullptr;
		for (auto op: *(this->_stack))
			if (min == nullptr || *min > *op)
				min = op;
		auto minv = &(min->toString());
		std::cout << *minv << std::endl;
		delete minv;
	} catch (AVMRuntimeError &ex) {
		throw ExecutorException("Minimum error: " + std::string(ex.what()));
	}
}

void Executor::pow() {
	try {
		this->_checkStack(2);
		auto iter = this->_stack->rbegin();
		auto rOperand = &(*iter++)->toString(),
			lOperand = &(*iter)->toString();
		delete *(iter - 1);
		delete *iter;
		this->_stack->pop_back();
		this->_stack->pop_back();
		auto ss = std::stringstream();
		ss << std::pow(boost::lexical_cast<double>(*lOperand),
					   boost::lexical_cast<double>(*rOperand));
		auto result = this->_operandFactory->createOperand(
				IOperand::eOperandType::Double, ss.str());
		this->_stack->push_back(result);
		delete lOperand;
		delete rOperand;
	} catch (AVMRuntimeError &ex) {
		throw ExecutorException("Power error: " + std::string(ex.what()));
	}
}

void Executor::cleanStack() {
	for (auto ptr: *this->_stack)
		delete ptr;
	this->_stack->clear();
}

void Executor::sqrt() {
	try{
		this->_checkStack(1);
		auto iter = this->_stack->rbegin();
		auto operand =  &(*iter)->toString();
		auto val = boost::lexical_cast<double>(*operand);
		if (val < 0) {
			delete operand;
			throw ExecutorException("Cannot be applied to negative number");
		}
		val = std::sqrt(val);
		delete operand;
		auto ss = std::stringstream();
		ss << val;
		delete *iter;
		*iter = _operandFactory->createOperand(IOperand::eOperandType::Double, ss.str());
	} catch (AVMRuntimeError &ex) {
		throw ExecutorException("Square root error: " + std::string(ex.what()));
	}
}

void Executor::log() {
	try{
		this->_checkStack(1);
		auto iter = this->_stack->rbegin();
		auto operand =  &(*iter)->toString();
		auto val = boost::lexical_cast<double>(*operand);
		if (val < 0) {
			delete operand;
			throw ExecutorException("Cannot be applied to negative number");
		}
		val = std::log(val);
		delete operand;
		auto ss = std::stringstream();
		ss << val;
		delete *iter;
		*iter = _operandFactory->createOperand(IOperand::eOperandType::Double, ss.str());
	} catch (AVMRuntimeError &ex) {
		throw ExecutorException("Log error: " + std::string(ex.what()));
	}
}

Executor::ExecutorException::ExecutorException(std::string message)
		: AVMRuntimeError(std::move(message)) {

}
