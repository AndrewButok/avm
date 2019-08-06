/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:15 by abutok            #+#    #+#             */
/*   Updated: 2019/08/05 12:41:01 by abutok           ###   ########.fr       */
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
	Parser(const Parser&);
	Parser &operator=(const Parser&);
	void _checkTokens(std::vector<TokenPtr>&);
	IOperandPtr makeOperand(TokenPtr &constructorToken, TokenPtr &rawValueToken);
	eTokenType _execute(std::vector<TokenPtr> &);
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
