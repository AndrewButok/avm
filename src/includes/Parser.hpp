/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:15 by abutok            #+#    #+#             */
/*   Updated: 2019/07/30 15:57:11 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_PARSER_HPP
#define AVM_PARSER_HPP

#include <set>
#include "OperandFactory.hpp"
#include "Executor.hpp"
#include "Lexer.hpp"

class Parser {
private:
	typedef Token::eTokenType eTokenType;
	typedef OperandFactory::eOperandType eOperandType;
	static Parser *_instance;


	OperandFactory	*_operandFactory;
	Executor		*_executor;
	std::set<eTokenType> _operators;
	std::set<eTokenType> _constructors;

	Parser();
	Parser(const Parser&) = default;
	Parser &operator=(const Parser&) = default;
	void _checkTokens(std::vector<Token *>&);
	static void _deleteTokens(std::vector<Token *> *);
	const IOperand *makeOperand(Token* constructorToken, Token* rawValueToken);
	eTokenType _execute(std::vector<Token *> &);
public:
	virtual ~Parser();
	static Parser *getInstance();

	eTokenType parse(const std::string &);
	void cleanExecutor();
};


#endif //AVM_PARSER_HPP
