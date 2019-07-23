/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:03:19 by abutok            #+#    #+#             */
/*   Updated: 2019/07/23 18:48:14 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "OperandFactory.hpp"
#include "Operand.hpp"

OperandFactory* OperandFactory::_instance = nullptr;

OperandFactory *OperandFactory::getInstance() {
	if (OperandFactory::_instance == nullptr)
		OperandFactory::_instance = new OperandFactory();
	return OperandFactory::_instance;
}

OperandFactory::OperandFactory() {
	this->_functions = new std::vector<CreateTypeFunc>();
	this->_functions->push_back(nullptr);
	this->_functions->push_back(&OperandFactory::createInt8);
	this->_functions->push_back(&OperandFactory::createInt16);
	this->_functions->push_back(&OperandFactory::createInt32);
	this->_functions->push_back(&OperandFactory::createFloat);
	this->_functions->push_back(&OperandFactory::createDouble);
}

const IOperand *OperandFactory::createInt8(const std::string &value) const {
	if (!OperandFactory::_isNumber(value))
		throw std::invalid_argument("Wrong Operand format");
	std::stringstream s(value);
	long double val;
	s >> val;
	if (val > static_cast<long double>(std::numeric_limits<char>::max()))
		throw std::overflow_error("Operand overflow");
	if (val < static_cast<long double>(std::numeric_limits<char>::min()))
		throw std::underflow_error("Operand underflow");
	return new Int8(static_cast<char>(val));
}

const IOperand *OperandFactory::createInt16(const std::string &value) const {
	if (!OperandFactory::_isNumber(value))
		throw std::invalid_argument("Wrong Operand format");
	std::stringstream s(value);
	long double val;
	s >> val;
	if (val > static_cast<long double>(std::numeric_limits<short>::max()))
		throw std::overflow_error("Operand overflow");
	if (val < static_cast<long double>(std::numeric_limits<short>::min()))
		throw std::underflow_error("Operand underflow");
	return new Int16(static_cast<short>(val));
}

const IOperand *OperandFactory::createInt32(const std::string &value) const {
	if (!OperandFactory::_isNumber(value))
		throw std::invalid_argument("Wrong Operand format");
	std::stringstream s(value);
	long double val;
	s >> val;
	if (val > static_cast<long double>(std::numeric_limits<int>::max()))
		throw std::overflow_error("Operand overflow");
	if (val < static_cast<long double>(std::numeric_limits<int>::min()))
		throw std::underflow_error("Operand underflow");
	return new Int32(static_cast<int>(val));
}

const IOperand *OperandFactory::createFloat(const std::string &value) const {
	if (!OperandFactory::_isNumber(value))
		throw std::invalid_argument("Wrong Operand format");
	std::stringstream s(value);
	long double val;
	s >> val;
	if (val > static_cast<long double>(std::numeric_limits<float>::max()))
		throw std::overflow_error("Operand overflow");
	if (val < static_cast<long double>(std::numeric_limits<float>::min()))
		throw std::underflow_error("Operand underflow");
	return new Float(static_cast<float>(val));
}

const IOperand *OperandFactory::createDouble(const std::string &value) const {
	if (!OperandFactory::_isNumber(value))
		throw std::invalid_argument("Wrong Operand format");
	std::stringstream s(value);
	long double val;
	s >> val;
	if (val > static_cast<long double>(std::numeric_limits<double>::max()))
		throw std::overflow_error("Operand overflow");
	if (val < static_cast<long double>(std::numeric_limits<double>::min()))
		throw std::underflow_error("Operand underflow");
	return new Double(static_cast<double>(val));
}

const IOperand *OperandFactory::createOperand(OperandFactory::eOperandType type,
											  const std::string &value) {
	CreateTypeFunc ctf = (*this->_functions)[static_cast<unsigned int>(type)];
	return (this->*ctf)(value);
}

bool OperandFactory::_isNumber(const std::string &value) {
	// BIG TRASH
	bool dot = false;
	for (char c: value) {
		if (!isnumber(static_cast<int>(c)) && c != '.')
			return false;
		if (c == '.' && dot)
			return false;
		if (c == '.')
			dot = true;
	}
	return true;
}

OperandFactory::~OperandFactory() {
	delete this->_functions;
}
