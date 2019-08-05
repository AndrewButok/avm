/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:15 by abutok            #+#    #+#             */
/*   Updated: 2019/08/04 11:16:10 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_PARSER_HPP
#define AVM_PARSER_HPP

#include <set>
#include "OperandFactory.hpp"
#include "Executor.hpp"
#include "Lexer.hpp"
#include "AVMRuntimeError.hpp"

class Parser {
private:
	typedef Token::eTokenType eTokenType;
	typedef OperandFactory::eOperandType eOperandType;
	static Parser *_instance;


	OperandFactory	*_operandFactory;
	Executor		*_executor;

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

	class ParserException: public AVMRuntimeError{
	public:
		explicit ParserException(std::string message);
	};

};


#endif //AVM_PARSER_HPP
