/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:03:13 by abutok            #+#    #+#             */
/*   Updated: 2019/08/02 16:02:47 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_OPERANDFACTORY_HPP
#define AVM_OPERANDFACTORY_HPP

#include "IOperand.hpp"

class OperandFactory {
private:
	typedef const IOperand* (OperandFactory::*CreateTypeFunc)(const std::string &) const;
	static OperandFactory *_instance;

	std::vector<CreateTypeFunc> *_functions;

	int _getPrecision(const std::string &value)const;

	const IOperand *createInt8(const std::string &value) const;
	const IOperand *createInt16(const std::string &value) const;
	const IOperand *createInt32(const std::string &value) const;
	const IOperand *createFloat(const std::string &value) const;
	const IOperand *createDouble(const std::string &value) const;

	OperandFactory();
	OperandFactory(const OperandFactory&) = default;
	OperandFactory &operator=(const OperandFactory&) = default;
public:
	typedef IOperand::eOperandType eOperandType;

	virtual ~OperandFactory();
	static OperandFactory *getInstance();
	const IOperand *createOperand(eOperandType type, const std::string &value);
	const IOperand *createOperand(IOperand::eOperandType type, const IOperand *);

};


#endif //AVM_OPERANDFACTORY_HPP
