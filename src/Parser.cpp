/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:26 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 13:46:06 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser* Parser::_instance = nullptr;

Parser::Parser(const Parser&) = default;
Parser &Parser::operator=(const Parser&) = default;

Parser::Parser() {
	_operandFactory = OperandFactory::getInstance();
	_executor = Executor::getInstance();
}

Parser::~Parser() {
	delete _executor;
}

Parser *Parser::getInstance() {
	if (_instance == nullptr)
		_instance = new Parser();
	return _instance;
}

Parser::eTokenType Parser::parse(const std::string &row) {
	auto tokens = Lexer::tokenize(row);
	try {
		_checkTokens(tokens);
		auto rv = _execute(tokens);
		return rv;
	} catch (AVMRuntimeError &ex) {
		_executor->kill();
		throw ex;
	}
}

void Parser::_checkTokens(std::vector<TokenPtr> &tokens) {
    if (tokens.empty())
        return;
    auto iter = tokens.begin();
    auto end = tokens.end();
	if (Token::_operators.find((*iter)->getType()) == Token::_operators.end())
		throw ParserException("Invalid operator");
	if ((((*iter)->getType() != eTokenType::Assert &&
            (*iter)->getType() != eTokenType::Push) &&
            (iter + 1) == end))
		return ;
	if ((iter + 1) != end && (*(iter + 1))->getType() != eTokenType::WS)
		throw ParserException("Invalid operator");
	if ((((*iter)->getType() == eTokenType::Assert ||
            (*iter)->getType() == eTokenType::Push) &&
            (iter + 1) == end)||
		((*iter)->getType() != eTokenType::Assert &&
		 tokens[0]->getType() != eTokenType::Push &&
                ((iter + 1) != end)))
		throw ParserException("Invalid number of arguments");
	iter++;
	if ((*iter)->getType() != eTokenType::WS)
		throw ParserException("Invalid operator");
	iter++;
	if (iter == end)
	    throw ParserException("No operator argument");
	if (Token::_constructors.find((*iter)->getType()) == Token::_constructors.end())
		throw ParserException("Invalid operator argument");
	iter++;
	if (iter == end ||
        (*iter)->getType() != eTokenType::OBrace)
		throw ParserException("Missing constructor opening bracket");
	iter++;
	if (iter == end ||
        (*iter)->getType() != eTokenType::RawValue)
		throw ParserException("Missing constructor value");
	iter++;
	if (iter == end ||
        (*iter)->getType() != eTokenType::CBrace)
		throw ParserException("Missing constructor closing bracket");
	iter++;
	if (iter != end &&
        (*iter)->getType() == eTokenType::WS)
		throw ParserException("Too many arguments");
	if (iter != end)
		throw ParserException("Invalid argument");
}

Parser::eTokenType Parser::_execute(std::vector<TokenPtr> &tokens) {
    if (tokens.empty())
        return  eTokenType::RawValue;
	IOperandPtr operandPtr;
	switch (tokens[0]->getType()){
		case eTokenType::Push:{
			operandPtr = IOperandPtr{makeOperand(tokens[2], tokens[4])};
			_executor->pushToStack(operandPtr);
			break;
		}
		case eTokenType::Assert:{
			operandPtr = IOperandPtr{makeOperand(tokens[2], tokens[4])};
			_executor->assertFromStack(operandPtr);
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
		case eTokenType::Min:
			_executor->min();
			break;
		case eTokenType::Max:
			_executor->max();
			break;
		case eTokenType::Pow:
			_executor->pow();
			break;
		case eTokenType::Sqrt:
			_executor->sqrt();
			break;
		case eTokenType::Log:
			_executor->log();
			break;
		case eTokenType::Exit:
			break;
		default:
			throw ParserException("Impossible. How you pass non operator token?");
	}
	return tokens[0]->getType();
}

IOperandPtr Parser::makeOperand(TokenPtr &constructorToken, TokenPtr &rawValueToken) {
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
		throw ParserException("Illegal constructor");
}

void Parser::cleanExecutor() {
	this->_executor->reset();
}

Parser::ParserException::ParserException(std::string message) : AVMRuntimeError(std::move(message)) {

}
