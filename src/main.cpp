/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 13:58:33 by abutok            #+#    #+#             */
/*   Updated: 2019/07/28 18:42:22 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <Token.hpp>
#include <Lexer.hpp>
#include "Executor.hpp"

int main() {
//	OperandFactory *of = OperandFactory::getInstance();
//	const IOperand *f1 = of->createOperand(IOperand::eOperandType::Int8, "45"),
//			*f2 = of->createOperand(IOperand::eOperandType::Int8, "25");
//	Executor *executor = Executor::getInstance();
//	executor->pushToStack(f1);
//	executor->pushToStack(f2);
//	executor->popFromStack();
//	executor->pushToStack(f1);
//	executor->add();
//	executor->dumpStack();
//	executor->print();
//	system("leaks Abstract-VM");
	std::vector<Token *> *tokens = Lexer::tokenize("                       push    Int32          (456    ..234.2.34.2234234234)");
	for (auto *token_ptr: *tokens)
		delete (token_ptr);
	delete tokens;
	system("leaks Abstract-VM");
	return 0;
}