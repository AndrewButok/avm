/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 18:51:02 by abutok            #+#    #+#             */
/*   Updated: 2019/08/04 11:36:33 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_EXECUTOR_HPP
#define AVM_EXECUTOR_HPP

#include <vector>
#include "AVMRuntimeError.hpp"
#include "IOperand.hpp"
#include "OperandFactory.hpp"

class Executor {
private:
	enum class eOperatorType: unsigned int;
	typedef const IOperand* (IOperand::*Operator)(const IOperand&) const;
	static Executor *_instance;
	std::vector<const IOperand *> *_stack;
	std::vector<Operator> *_operators;
	OperandFactory *_operandFactory;

	Executor();
	Executor(const Executor &executor) = default;
	Executor &operator=(const Executor&) = default;
	void _checkStack(unsigned int) const;
	void _executeArithmeticOperator(eOperatorType type);

	enum class eOperatorType: unsigned int {
		Add = 0,
		Sub = 1,
		Mul = 2,
		Div = 3,
		Mod = 4
	};

public:
	virtual ~Executor();
	static Executor *getInstance();
	void cleanStack();

	void pushToStack(const IOperand *operand);
	void popFromStack();
	void dumpStack();
	void assertFromStack(const IOperand *operand);
	void add();
	void sub();
	void mul();
	void div();
	void mod();
	void print();
	void max();
	void min();
	void pow();
	void sqrt();
	void log();

	class ExecutorException: public AVMRuntimeError{
		public:
			explicit ExecutorException(std::string message);
	};
};


#endif //AVM_EXECUTOR_HPP
