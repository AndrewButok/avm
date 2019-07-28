/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 13:58:33 by abutok            #+#    #+#             */
/*   Updated: 2019/07/25 17:39:30 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <Token.hpp>
#include "Executor.hpp"

int main() {
	OperandFactory *of = OperandFactory::getInstance();
	const IOperand *f1 = of->createOperand(IOperand::eOperandType::Int8, "45"),
			*f2 = of->createOperand(IOperand::eOperandType::Int8, "25");
	Executor *executor = Executor::getInstance();
	executor->pushToStack(f1);
	executor->pushToStack(f2);
	executor->popFromStack();
	executor->pushToStack(f1);
	executor->add();
	executor->dumpStack();
	executor->print();
	system("leaks Abstract-VM");
	return 0;
}