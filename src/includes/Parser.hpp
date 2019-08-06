/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:15 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 20:32:08 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_PARSER_HPP
#define AVM_PARSER_HPP

#include "OperandFactory.hpp"
#include "Executor.hpp"
#include "Lexer.hpp"
#include "AVMRuntimeError.hpp"

class Parser;

using ParserPtr = std::shared_ptr<Parser>;

class Parser {
private:
	typedef Token::eTokenType eTokenType;
	typedef OperandFactory::eOperandType eOperandType;
	static ParserPtr _instance;


	OperandFactoryPtr	_operandFactory;
	ExecutorPtr			_executor;

	Parser();
	Parser(const Parser&);
	Parser &operator=(const Parser&);
	void _checkTokens(std::vector<TokenPtr>&);
	IOperandPtr makeOperand(TokenPtr &constructorToken, TokenPtr &rawValueToken);
	eTokenType _execute(std::vector<TokenPtr> &);
public:
	virtual ~Parser();
	static ParserPtr getInstance();

	eTokenType parse(const std::string &);
	void cleanExecutor();

	class ParserException: public AVMRuntimeError{
	public:
		explicit ParserException(std::string message);
	};

};


#endif //AVM_PARSER_HPP
