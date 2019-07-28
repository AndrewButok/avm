/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:26 by abutok            #+#    #+#             */
/*   Updated: 2019/07/28 20:17:42 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include "Token.hpp"
#include "Parser.hpp"

Parser::Parser() {
	_operandFactory = OperandFactory::getInstance();
	_executor = Executor::getInstance();
}

Parser::~Parser() {
	delete _operandFactory;
	delete _executor;
}

Parser *Parser::getInstance() {
	if (instance == nullptr)
		instance = new Parser();
	return instance;
}

void Parser::parse(const std::string &row) {
	auto *tokens = Lexer::tokenize(row);
	auto &tokens_ref = *tokens;
	try {
		_checkTokens(tokens_ref);
	} catch (std::exception &ex) {
		_deleteTokens(tokens);
		throw ex;
	}
}

void Parser::_checkTokens(std::vector<Token *> &tokens) {

}

void Parser::_deleteTokens(std::vector<Token *> *tokens) {
	if (!tokens->empty()) {
		for(auto token_ref: *tokens)
			delete token_ref;
	}
	delete tokens;
}
