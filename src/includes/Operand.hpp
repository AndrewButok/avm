/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:24:43 by abutok            #+#    #+#             */
/*   Updated: 2019/08/05 11:03:44 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_OPERAND_HPP
#define AVM_OPERAND_HPP

#include <sstream>
#include <cmath>
#include <iomanip>
#include "IOperand.hpp"

template <typename Base>
class Operand: public IOperand{
private:
	static void		_checkType(const IOperand &lo, const IOperand &ro);
	Base			_val;
	int				_precision;
public:
	//
	//Canonical form
	//
	Operand();
	explicit Operand(Base val, int precision);
	Operand(const Operand<Base> &base);
	Operand<Base> &operator=(const Operand<Base> &);
	~Operand() override = default;

	char getValue() const;

	//
	//IOperand functions
	//
	int					getPrecision() const override;
	eOperandType 		getType() const override;

	const IOperand		*operator+(const IOperand &) const override ;
	const IOperand		*operator-(const IOperand &) const override ;
	const IOperand		*operator/(const IOperand &) const override ;
	const IOperand		*operator*(const IOperand &) const override ;
	const IOperand		*operator%(const IOperand &) const override ;
	bool				operator>(const IOperand &operand) const override;
	bool				operator<(const IOperand &operand) const override;

	const std::string	&toString() const override;

	class OperandException: public AVMRuntimeError{
	public:
		explicit OperandException(std::string message) : AVMRuntimeError(std::move(message)) {
		}
	};
};

template<typename Base>
Operand<Base>::Operand(): _val(0), _precision(0) {}

template<typename Base>
Operand<Base>::Operand(Base val, int precision): _val(val) {
	if (precision < 0)
		_precision = 0;
	else
		_precision = precision;
}

template<typename Base>
Operand<Base>::Operand(const Operand<Base> &base): _val(base._val), _precision(base._precision) {}

template<typename Base>
Operand<Base> &Operand<Base>::operator=(const Operand<Base> &base) {
	this->_val = base._val;
	return *this;
}

template<typename Base>
char Operand<Base>::getValue() const {
	return this->_val;
}

template<typename Base>
int Operand<Base>::getPrecision() const {
	return _precision;
}

template<typename Base>
IOperand::eOperandType Operand<Base>::getType() const {
	return IOperand::eOperandType::UnknownOperand;
}

template<>
IOperand::eOperandType Operand<char>::getType() const {
	return IOperand::eOperandType::Int8;
}

template<>
IOperand::eOperandType Operand<short>::getType() const {
	return IOperand::eOperandType::Int16;
}

template<>
IOperand::eOperandType Operand<int>::getType() const {
	return IOperand::eOperandType::Int32;
}

template<>
IOperand::eOperandType Operand<float>::getType() const {
	return IOperand::eOperandType::Float;
}

template<>
IOperand::eOperandType Operand<double>::getType() const {
	return IOperand::eOperandType::Double;
}

template<typename Base>
const IOperand *Operand<Base>::operator+(const IOperand &right_operand) const {
	Operand<Base>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<Base> &>(right_operand);
	if (casted_ro._val > 0 &&
		this->_val > std::numeric_limits<Base>::max() - casted_ro._val)
		throw OperandException("Operand overflow");
	if (casted_ro._val < 0 &&
		this->_val < std::numeric_limits<Base>::min() - casted_ro._val)
		throw OperandException("Operand underflow");
	return  new Operand<Base>(this->_val + casted_ro._val, 0);
}
template <>
const IOperand *Operand<float>::operator+(const IOperand &right_operand) const {
	Operand<float>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<float> &>(right_operand);
	return  new Operand<float>(this->_val + casted_ro._val,
							this->_precision < casted_ro._precision ? casted_ro._precision : this->_precision);
}

template <>
const IOperand *Operand<double>::operator+(const IOperand &right_operand) const {
	Operand<double>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<double> &>(right_operand);
	return  new Operand<double>(this->_val + casted_ro._val,
							this->_precision < casted_ro._precision ? casted_ro._precision : this->_precision);
}

template<typename Base>
const IOperand *Operand<Base>::operator-(const IOperand &right_operand) const {
	Operand<Base>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<Base> &>(right_operand);
	if (casted_ro._val > 0 &&
		this->_val < std::numeric_limits<Base>::min() + casted_ro._val)
		throw OperandException("Operand underflow");
	if (casted_ro._val < 0 &&
		this->_val > std::numeric_limits<Base>::max() + casted_ro._val)
		throw OperandException("Operand overflow");
	return  new Operand<Base>(this->_val - casted_ro._val, 0);
}

template<>
const IOperand *Operand<float>::operator-(const IOperand &right_operand) const {
	Operand<float>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<float> &>(right_operand);
	return  new Operand<float>(this->_val - casted_ro._val,
			this->_precision < casted_ro._precision ? casted_ro._precision : this->_precision);
}

template<>
const IOperand *Operand<double>::operator-(const IOperand &right_operand) const {
	Operand<double>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<double> &>(right_operand);
	return  new Operand<double>(this->_val - casted_ro._val,
			this->_precision < casted_ro._precision ? casted_ro._precision : this->_precision);
}

template<typename Base>
const IOperand *Operand<Base>::operator/(const IOperand &right_operand) const {
	Operand<Base>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<Base> &>(right_operand);
	if (casted_ro._val == 0)
		throw OperandException("Division by a zero");
	auto result = new Operand<Base>(this->_val / casted_ro._val, 0);
	return result;
}

template<>
const IOperand *Operand<float>::operator/(const IOperand &right_operand) const {
	Operand<float>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<float> &>(right_operand);
	auto result = new Operand<float>(this->_val / casted_ro._val, this->_precision + casted_ro._precision);
	return result;
}

template<>
const IOperand *Operand<double>::operator/(const IOperand &right_operand) const {
	Operand<double>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<double> &>(right_operand);
	auto result = new Operand<double>(this->_val / casted_ro._val, this->_precision + casted_ro._precision);
	return result;
}


template<typename Base>
const IOperand *Operand<Base>::operator*(const IOperand &right_operand) const {
	Operand<Base>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<Base> &>(right_operand);
	if ((casted_ro._val > 0 && this->_val > 0
		 && this->_val > std::numeric_limits<Base>::max() / casted_ro._val)
		|| (casted_ro._val < 0 && this->_val < 0
			&&
			this->_val < std::numeric_limits<Base>::max() / casted_ro._val))
		throw OperandException("Operand overflow");
	if ((casted_ro._val > 0 && this->_val < 0
		 && this->_val > std::numeric_limits<Base>::min() / casted_ro._val)
		|| (casted_ro._val < 0 && this->_val > 0
			&&
			this->_val < std::numeric_limits<Base>::min() / casted_ro._val))
		throw OperandException("Operand underflow");
	auto result = new Operand<Base>(this->_val * casted_ro._val, 0);
	return result;
}

template<>
const IOperand *Operand<float>::operator*(const IOperand &right_operand) const {
	Operand<float>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<float> &>(right_operand);
	auto result = new Operand<float>(this->_val * casted_ro._val,
									 this->_precision + casted_ro._precision);
	return result;
}

template<>
const IOperand *Operand<double>::operator*(const IOperand &right_operand) const {
	Operand<double>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<double> &>(right_operand);
	auto result = new Operand<double>(this->_val * casted_ro._val,
									  this->_precision + casted_ro._precision);
	return result;
}

template<typename Base>
const IOperand *Operand<Base>::operator%(const IOperand &right_operand) const {
	Operand<Base>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<Base> &>(right_operand);
	if (casted_ro._val == 0)
		throw OperandException("Division by a zero");
	auto result = new Operand<Base>(this->_val % casted_ro._val, 0);
	return result;
}

template<>
const IOperand *Operand<float>::operator%(const IOperand &right_operand) const {
	Operand<float>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<float> &>(right_operand);
	auto result = new Operand<float>(fmod(this->_val, casted_ro._val),
			this->_precision < casted_ro._precision ? casted_ro._precision : this->_precision);
	return result;
}

template<>
const IOperand *Operand<double>::operator%(const IOperand &right_operand) const {
	Operand<double>::_checkType(*this, right_operand);
	auto casted_ro = dynamic_cast<const Operand<double> &>(right_operand);
	auto result = new Operand<double>(fmod(this->_val, casted_ro._val),
			this->_precision < casted_ro._precision ? casted_ro._precision : this->_precision);
	return result;
}

template<typename Base>
const std::string &Operand<Base>::toString() const {
	std::stringstream ss;
	ss << this->_val;
	return *(new std::string(ss.str()));
}

template<>
const std::string &Operand<char>::toString() const {
	std::stringstream ss;
	ss << static_cast<int>(this->_val);
	return *(new std::string(ss.str()));
}

template<>
const std::string &Operand<double>::toString() const {
	std::stringstream ss;
	if (this->_precision != 0)
		ss << std::setprecision(this->_precision) << std::fixed <<this->_val;
	else
		ss << this->_val;
	auto result = new std::string(ss.str());
	return *result;
}

template<>
const std::string &Operand<float>::toString() const {
	std::stringstream ss;
	if (this->_precision != 0)
		ss << std::setprecision( this->_precision) << std::fixed <<this->_val;
	else
		ss << this->_val;
	auto result = new std::string(ss.str());
	return *result;
}

template<typename Base>
void Operand<Base>::_checkType(const IOperand &lo, const IOperand& ro) {
	if (lo.getType() == IOperand::eOperandType::UnknownOperand ||
		ro.getType() == IOperand::eOperandType::UnknownOperand ||
		lo.getType() != ro.getType())
		throw OperandException("Wrong operand eType");
}

template<typename Base>
bool Operand<Base>::operator<(const IOperand &operand) const {
	const std::string *thisStr = &(this->toString());
	const std::string *operandStr = &(operand.toString());
	auto a = boost::lexical_cast<double>(*thisStr),
			b = boost::lexical_cast<double>(*operandStr);
	delete thisStr;
	delete operandStr;
	return  a < b;
}

template<typename Base>
bool Operand<Base>::operator>(const IOperand &operand) const {
	const std::string *thisStr = &(this->toString());
	const std::string *operandStr = &(operand.toString());
	auto a = boost::lexical_cast<double>(*thisStr),
			b = boost::lexical_cast<double>(*operandStr);
	delete thisStr;
	delete operandStr;
	return a > b;
}

typedef Operand<char> Int8;
typedef Operand<short> Int16;
typedef Operand<int> Int32;
typedef Operand<float> Float;
typedef Operand<double> Double;

#endif //AVM_OPERAND_HPP
