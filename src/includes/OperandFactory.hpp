/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OperandFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 15:03:13 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 20:32:08 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_OPERANDFACTORY_HPP
#define AVM_OPERANDFACTORY_HPP

#include "IOperand.hpp"
#include "AVMRuntimeError.hpp"

class OperandFactory;

using OperandFactoryPtr = std::shared_ptr<OperandFactory>;

class OperandFactory {
private:
	typedef IOperandPtr (OperandFactory::*CreateTypeFunc)(const std::string &) const;
	static OperandFactoryPtr _instance;

	std::unique_ptr<std::vector<CreateTypeFunc>> _functions;

	int _getPrecision(const std::string &value)const;

	IOperandPtr createInt8(const std::string &value) const;
	IOperandPtr createInt16(const std::string &value) const;
	IOperandPtr createInt32(const std::string &value) const;
	IOperandPtr createFloat(const std::string &value) const;
	IOperandPtr createDouble(const std::string &value) const;

	OperandFactory();
	OperandFactory(const OperandFactory&);
	OperandFactory &operator=(const OperandFactory&);
public:
	typedef IOperand::eOperandType eOperandType;

	virtual ~OperandFactory();
	static OperandFactoryPtr getInstance();
	IOperandPtr createOperand(eOperandType type, const std::string &value);
	IOperandPtr createOperand(IOperand::eOperandType type, IOperandPtr &);

	class OperandFactoryException: public AVMRuntimeError{
	public:
		explicit OperandFactoryException(std::string message);
	};
};

#endif //AVM_OPERANDFACTORY_HPP
