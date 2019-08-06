/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 18:51:02 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 20:32:31 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_EXECUTOR_HPP
#define AVM_EXECUTOR_HPP

#include <vector>
#include "AVMRuntimeError.hpp"
#include "IOperand.hpp"
#include "OperandFactory.hpp"

class Executor;

using ExecutorPtr = std::shared_ptr<Executor>;

class Executor {
private:
	enum class eOperatorType: unsigned int;
	typedef IOperandPtr (IOperand::*Operator)(const IOperand&) const;
	static ExecutorPtr _instance;
	bool _alive;
	std::vector<IOperandPtr> _stack;
	std::vector<Operator> _operators;
	OperandFactoryPtr _operandFactory;

	Executor();
	Executor(const Executor &executor);
	Executor &operator=(const Executor&);
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
	static ExecutorPtr getInstance();
	void reset();
	void kill();

	void pushToStack(IOperandPtr &);
	void popFromStack();
	void dumpStack();
	void assertFromStack(IOperandPtr &);
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
