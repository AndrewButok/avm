/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:03:19 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 16:09:57 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <boost/lexical_cast.hpp>
#include "OperandFactory.hpp"
#include "Operand.hpp"

OperandFactoryPtr OperandFactory::_instance = nullptr;

OperandFactory::OperandFactory(const OperandFactory&){
};
OperandFactory &OperandFactory::operator=(const OperandFactory&){
	return *this;
};

OperandFactoryPtr OperandFactory::getInstance() {
	if (!OperandFactory::_instance)
		OperandFactory::_instance = OperandFactoryPtr {new OperandFactory()};
	return OperandFactory::_instance;
}

OperandFactory::OperandFactory() {
	this->_functions = std::make_unique<std::vector<CreateTypeFunc>>();
	this->_functions->push_back(nullptr);
	this->_functions->push_back(&OperandFactory::createInt8);
	this->_functions->push_back(&OperandFactory::createInt16);
	this->_functions->push_back(&OperandFactory::createInt32);
	this->_functions->push_back(&OperandFactory::createFloat);
	this->_functions->push_back(&OperandFactory::createDouble);
}

IOperandPtr OperandFactory::createInt8(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<int>(value);
		if (val > static_cast<int>(std::numeric_limits<char>::max()))
			throw boost::bad_lexical_cast();
		if (val < static_cast<int>(std::numeric_limits<char>::min()))
			throw boost::bad_lexical_cast();
		return std::make_unique<Int8>(static_cast<char>(val), 0);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Int8");
	}
}

IOperandPtr OperandFactory::createInt16(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<short>(value);
		return std::make_unique<Int16>(val, 0);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Int16");
	}
}

IOperandPtr OperandFactory::createInt32(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<int>(value);
		return std::make_unique<Int32>(val, 0);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Int32");
	}
}

IOperandPtr OperandFactory::createFloat(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<float>(value);
		auto precision = _getPrecision(value);
		return std::make_unique<Float>(val, precision);
	} catch (boost::bad_lexical_cast &ex){
		throw OperandFactoryException("Value could not be interpreted as Float");
	}
}

IOperandPtr OperandFactory::createDouble(const std::string &value) const {
	try {
		auto val = boost::lexical_cast<double>(value);
		auto precision = _getPrecision(value);
		return std::make_unique<Double>(val, precision);
	} catch (boost::bad_lexical_cast &ex){
		throw AVMRuntimeError("Value could not be interpreted as Double");
	}
}

IOperandPtr OperandFactory::createOperand(OperandFactory::eOperandType type,
											  const std::string &value) {
	if (type == eOperandType::UnknownOperand)
		throw OperandFactoryException("Factory method have token invalid operand eType");
	auto ctf = (*this->_functions)[static_cast<unsigned int>(type)];
	return (this->*ctf)(value);
}

OperandFactory::~OperandFactory() {
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
