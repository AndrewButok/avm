/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOperand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 23:29:14 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 13:36:11 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_IOPERAND_HPP
#define AVM_IOPERAND_HPP

#include <string>

class IOperand {
public:
	enum class eOperandType: unsigned int;

	virtual int					getPrecision() const = 0;
	virtual eOperandType		getType() const = 0;

	virtual const IOperand		*operator+(const IOperand &) const = 0;
	virtual const IOperand		*operator-(const IOperand &) const = 0;
	virtual const IOperand		*operator/(const IOperand &) const = 0;
	virtual const IOperand		*operator*(const IOperand &) const = 0;
	virtual const IOperand		*operator%(const IOperand &) const = 0;
	virtual bool				operator>(const IOperand &) const = 0;
	virtual bool				operator<(const IOperand &) const = 0;

	virtual const std::string	&toString() const = 0;

	virtual 					~IOperand() = default;

	enum class eOperandType: unsigned int{
		UnknownOperand = 0,
		Int8 = 1,
		Int16 = 2,
		Int32 = 3,
		Float = 4,
		Double = 5
	};
};

using IOperandPtr = std::unique_ptr<const IOperand>;
using StrPtr = std::unique_ptr<const std::string>;

#endif //AVM_IOPERAND_HPP
