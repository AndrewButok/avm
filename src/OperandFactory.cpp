/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:03:19 by abutok            #+#    #+#             */
/*   Updated: 2019/07/25 15:01:43 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <boost/lexical_cast.hpp>
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
	try {
		int val = boost::lexical_cast<int>(value);
		if (val > static_cast<int>(std::numeric_limits<char>::max()))
			throw boost::bad_lexical_cast();
		if (val < static_cast<int>(std::numeric_limits<char>::min()))
			throw boost::bad_lexical_cast();
		return new Int8(static_cast<char>(val));
	} catch (boost::bad_lexical_cast &ex){
		throw std::invalid_argument("Int8 bad value");
	}
}

const IOperand *OperandFactory::createInt16(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<short>(value);
		return new Int16(val);
	} catch (boost::bad_lexical_cast &ex){
		throw std::invalid_argument("Int16 bad value");
	}
}

const IOperand *OperandFactory::createInt32(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<int>(value);
		return new Int32(val);
	} catch (boost::bad_lexical_cast &ex){
		throw std::invalid_argument("Int32 bad value");
	}
}

const IOperand *OperandFactory::createFloat(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<float>(value);
		return new Float(val);
	} catch (boost::bad_lexical_cast &ex){
		throw std::invalid_argument("Float bad value");
	}
}

const IOperand *OperandFactory::createDouble(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<double>(value);
		return new Double(val);
	} catch (boost::bad_lexical_cast &ex){
		throw std::invalid_argument("Double bad value");
	}
}

const IOperand *OperandFactory::createOperand(OperandFactory::eOperandType type,
											  const std::string &value) {
	if (type == eOperandType::UnknownOperand)
		throw std::invalid_argument("Factory method have token invalid operand type");
	CreateTypeFunc ctf = (*this->_functions)[static_cast<unsigned int>(type)];
	return (this->*ctf)(value);
}

OperandFactory::~OperandFactory() {
	delete this->_functions;
}

const IOperand *OperandFactory::createOperand(IOperand::eOperandType type, const IOperand *operand) {
	if (operand == nullptr)
		return nullptr;
	if (operand->getType() == IOperand::eOperandType::UnknownOperand ||
		operand->getType() >= type)
		throw std::runtime_error("Operand type is greater than type sent to function");
	const IOperand *result = nullptr;
	const std::string *strOperandVal = &(operand->toString());
	try {
		switch (type){
			case IOperand::eOperandType::Int8:
				result = this->createOperand(IOperand::eOperandType ::Int8, *strOperandVal);
				break;
			case IOperand::eOperandType::Int16:
				result = this->createOperand(IOperand::eOperandType ::Int16, *strOperandVal);
				break;
			case IOperand::eOperandType::Int32:
				result = this->createOperand(IOperand::eOperandType ::Int32, *strOperandVal);
				break;
			case IOperand::eOperandType::Float:
				result = this->createOperand(IOperand::eOperandType ::Float, *strOperandVal);
				break;
			case IOperand::eOperandType::Double:
				result = this->createOperand(IOperand::eOperandType ::Float, *strOperandVal);
				break;
			default:
				throw std::runtime_error("Unknown copy error");
		}
		delete strOperandVal;
		return result;
	} catch (std::exception &ex){
		delete strOperandVal;
		throw ex;
	}
}
