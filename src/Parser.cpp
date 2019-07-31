/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:26 by abutok            #+#    #+#             */
/*   Updated: 2019/07/30 23:42:10 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser* Parser::_instance = nullptr;

Parser::Parser() {
	_operandFactory = OperandFactory::getInstance();
	_executor = Executor::getInstance();
	_operators.insert(eTokenType::Push);
	_operators.insert(eTokenType::Assert);
	_operators.insert(eTokenType::Pop);
	_operators.insert(eTokenType::Dump);
	_operators.insert(eTokenType::Add);
	_operators.insert(eTokenType::Sub);
	_operators.insert(eTokenType::Mul);
	_operators.insert(eTokenType::Div);
	_operators.insert(eTokenType::Mod);
	_operators.insert(eTokenType::Print);
	_operators.insert(eTokenType::Exit);
	_constructors.insert(eTokenType::ConstructorInt8);
	_constructors.insert(eTokenType::ConstructorInt16);
	_constructors.insert(eTokenType::ConstructorInt32);
	_constructors.insert(eTokenType::ConstructorFloat);
	_constructors.insert(eTokenType::ConstructorDouble);
}

Parser::~Parser() {
	delete _operandFactory;
	delete _executor;
}

Parser *Parser::getInstance() {
	if (_instance == nullptr)
		_instance = new Parser();
	return _instance;
}

Parser::eTokenType Parser::parse(const std::string &row) {
	auto *tokens = Lexer::tokenize(row);
	auto &tokens_ref = *tokens;
	try {
		_checkTokens(tokens_ref);
		auto rv = _execute(tokens_ref);
		_deleteTokens(tokens);
		return rv;
	} catch (std::runtime_error &ex) {
		_deleteTokens(tokens);
		throw ex;
	}
}

void Parser::_checkTokens(std::vector<Token *> &tokens) {
	if (_operators.find(tokens[0]->getType()) == _operators.end())
		throw std::runtime_error("Invalid operator");
	if (((tokens[0]->getType() != eTokenType::Assert &&
		  tokens[0]->getType() != eTokenType::Push) &&
		 tokens.size() == 1))
		return ;
	if (((tokens[0]->getType() == eTokenType::Assert ||
		tokens[0]->getType() == eTokenType::Push) &&
		tokens.size() == 1)||
		((tokens[0]->getType() != eTokenType::Assert &&
		 tokens[0]->getType() != eTokenType::Push) &&
		tokens.size() > 1))
		throw std::runtime_error("Invalid number of arguments");
	if (tokens[1]->getType() != eTokenType::WS)
		throw std::runtime_error("Invalid operator");
	if (tokens.size() > 2 &&
		_constructors.find(tokens[2]->getType()) == _constructors.end())
		throw std::runtime_error("Invalid operator argument");
	if (tokens.size() > 3 &&
		tokens[3]->getType() != eTokenType::OBrace)
		throw std::runtime_error("Missing constructor opening bracket");
	if (tokens.size() > 4 &&
		tokens[4]->getType() != eTokenType::RawValue)
		throw std::runtime_error("Missing constructor value");
	if (tokens.size() > 5 &&
		tokens[5]->getType() != eTokenType::CBrace)
		throw std::runtime_error("Missing constructor closing bracket");
	if (tokens.size() > 6 && tokens[6]->getType() == eTokenType::WS)
		throw std::runtime_error("Too many arguments");
	if (tokens.size() > 6 && tokens[6]->getType() != eTokenType::WS)
		throw std::runtime_error("Invalid argument");
}

Parser::eTokenType Parser::_execute(std::vector<Token *> &tokens) {
	const IOperand *operand = nullptr;
	switch (tokens[0]->getType()){
		case eTokenType::Push:{
			operand = makeOperand(tokens[2], tokens[4]);
			_executor->pushToStack(operand);
			break;
		}
		case eTokenType::Assert:{
			operand = makeOperand(tokens[2], tokens[4]);
			_executor->pushToStack(operand);
			break;
		}
		case eTokenType::Pop:
			_executor->popFromStack();
			break;
		case eTokenType::Dump:
			_executor->dumpStack();
			break;
		case eTokenType::Add:
			_executor->add();
			break;
		case eTokenType::Sub:
			_executor->sub();
			break;
		case eTokenType::Mul:
			_executor->mul();
			break;
		case eTokenType::Div:
			_executor->div();
			break;
		case eTokenType::Mod:
			_executor->mod();
			break;
		case eTokenType::Print:
			_executor->print();
			break;
		case eTokenType::Exit:
			break;
		default:
			throw std::runtime_error("Impossible. How you pass non operator token?");
	}
	return tokens[0]->getType();
}

void Parser::_deleteTokens(std::vector<Token *> *tokens) {
	if (!tokens->empty()) {
		for(auto token_ref: *tokens)
			delete token_ref;
	}
	delete tokens;
}

const IOperand *
Parser::makeOperand(Token *constructorToken, Token *rawValueToken) {
	if (constructorToken->getType() == eTokenType::ConstructorInt8)
		return _operandFactory->createOperand(eOperandType::Int8, rawValueToken->getValue());
	if (constructorToken->getType() == eTokenType::ConstructorInt16)
		return _operandFactory->createOperand(eOperandType::Int16, rawValueToken->getValue());
	if (constructorToken->getType() == eTokenType::ConstructorInt32)
		return _operandFactory->createOperand(eOperandType::Int32, rawValueToken->getValue());
	if (constructorToken->getType() == eTokenType::ConstructorDouble)
		return _operandFactory->createOperand(eOperandType::Double, rawValueToken->getValue());
	if (constructorToken->getType() == eTokenType::ConstructorFloat)
		return _operandFactory->createOperand(eOperandType::Float, rawValueToken->getValue());
	else
		throw std::runtime_error("Illegal constructor");
}
