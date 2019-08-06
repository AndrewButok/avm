/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:03:19 by abutok            #+#    #+#             */
/*   Updated: 2019/08/04 13:27:42 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <boost/lexical_cast.hpp>
#include "OperandFactory.hpp"
#include "Operand.hpp"

OperandFactory* OperandFactory::_instance = nullptr;

OperandFactory::OperandFactory(const OperandFactory&) = default;
OperandFactory &OperandFactory::operator=(const OperandFactory&) = default;

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
		auto val = boost::lexical_cast<int>(value);
		if (val > static_cast<int>(std::numeric_limits<char>::max()))
			throw boost::bad_lexical_cast();
		if (val < static_cast<int>(std::numeric_limits<char>::min()))
			throw boost::bad_lexical_cast();
		return new Int8(static_cast<char>(val), 0);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Int8");
	}
}

const IOperand *OperandFactory::createInt16(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<short>(value);
		return new Int16(val, 0);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Int16");
	}
}

const IOperand *OperandFactory::createInt32(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<int>(value);
		return new Int32(val, 0);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Int32");
	}
}

const IOperand *OperandFactory::createFloat(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<float>(value);
		auto precision = _getPrecision(value);
		return new Float(val, precision);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Float");
	}
}

const IOperand *OperandFactory::createDouble(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<double>(value);
		auto precision = _getPrecision(value);
		return new Double(val, precision);
	} catch (boost::bad_lexical_cast &ex){
		throw AVMRuntimeError("Value could not be interpreted as Double");
	}
}

IOperandPtr OperandFactory::createOperand(OperandFactory::eOperandType type,
											  const std::string &value) {
	if (type == eOperandType::UnknownOperand)
		throw OperandFactoryException("Factory method have token invalid operand eType");
	auto ctf = (*this->_functions)[static_cast<unsigned int>(type)];
	return IOperandPtr{((this->*ctf)(value))};
}

OperandFactory::~OperandFactory() {
	delete this->_functions;
}

IOperandPtr OperandFactory::createOperand(IOperand::eOperandType type, IOperandPtr &operandPtr) {
	if (!operandPtr)
		return IOperandPtr();
	if (operandPtr->getType() == IOperand::eOperandType::UnknownOperand ||
		operandPtr->getType() >= type)
		throw OperandFactoryException("Operand eType is greater than eType sent to function");
	auto strOperandVal = std::unique_ptr<const std::string>{&(operandPtr->toString())};
	auto ctf = (*this->_functions)[static_cast<unsigned int>(type)];
	return IOperandPtr{((this->*ctf)(*strOperandVal))};
}

int OperandFactory::_getPrecision(const std::string &value) const {
	auto i = value.find('.');
	if (i != std::string::npos) {
		auto count = 0;
		while (isdigit(value[i + count + 1]))
			count++;
		if (value[i + count + 1] == 'e'){
			auto exponent_str = value.substr(i + count + 2);
			auto exponent = boost::lexical_cast<int>(exponent_str);
			if (exponent < 0)
				count -= exponent;
		}
		return count;
	}
	if ((i = value.find('e')) != std::string::npos ||
		(i = value.find('E')) != std::string::npos) {
		auto exponent_str = value.substr(i + 1);
		auto exponent = boost::lexical_cast<int>(exponent_str);
		if (exponent < 0)
			return -exponent;
	}
	return 0;
}

OperandFactory::OperandFactoryException::OperandFactoryException(
		std::string message) : AVMRuntimeError(std::move(message)) {

}
