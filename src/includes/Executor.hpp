/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 18:51:02 by abutok            #+#    #+#             */
/*   Updated: 2019/07/25 02:01:23 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_EXECUTOR_HPP
#define AVM_EXECUTOR_HPP

#include <vector>
#include "IOperand.hpp"
#include "OperandFactory.hpp"

class Executor {
private:
	static Executor *_instance;
	std::vector<const IOperand *> *_stack;
	OperandFactory *_operandFactory;

	Executor();
	Executor(const Executor &executor) = default;
	Executor &operator=(const Executor&) = default;
	void _checkStack(int) const;
public:
	virtual ~Executor();
	static Executor *getInstance();

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
};


#endif //AVM_EXECUTOR_HPP
