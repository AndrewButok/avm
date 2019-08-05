/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:24:21 by abutok            #+#    #+#             */
/*   Updated: 2019/08/05 12:41:01 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

std::map<std::string, Token::eTokenType> Token::_nonValueTokens = {
	{"push", eTokenType::Push},
	{"assert", eTokenType::Assert},
	{"pop", eTokenType::Pop},
	{"dump", eTokenType::Dump},
	{"add", eTokenType::Add},
	{"sub", eTokenType::Sub},
	{"mul", eTokenType::Mul},
	{"div", eTokenType::Div},
	{"mod", eTokenType::Mod},
	{"print", eTokenType::Print},
	{"exit", eTokenType::Exit},
	{"int8", eTokenType::ConstructorInt8},
	{"int16", eTokenType::ConstructorInt16},
	{"int32", eTokenType::ConstructorInt32},
	{"float", eTokenType::ConstructorFloat},
	{"double", eTokenType::ConstructorDouble},
	{"(", eTokenType::OBrace},
	{")", eTokenType::CBrace},
	{";", eTokenType::CommentBegin},
	{"min", eTokenType::Min},
	{"max", eTokenType::Max},
	{"pow", eTokenType::Pow},
	{"sqrt", eTokenType::Sqrt},
	{"log", eTokenType::Log},
	{" ", eTokenType::WS},
	{"\t", eTokenType::WS}
};

std::set<Token::eTokenType> Token::_operators = {
		eTokenType::Push,
		eTokenType::Assert,
		eTokenType::Pop,
		eTokenType::Dump,
		eTokenType::Add,
		eTokenType::Sub,
		eTokenType::Mul,
		eTokenType::Div,
		eTokenType::Mod,
		eTokenType::Print,
		eTokenType::Exit,
		eTokenType::Min,
		eTokenType::Max,
		eTokenType::Pow,
		eTokenType::Sqrt,
		eTokenType::Log
};
std::set<Token::eTokenType> Token::_constructors = {
		eTokenType::ConstructorInt8,
		eTokenType::ConstructorInt16,
		eTokenType::ConstructorInt32,
		eTokenType::ConstructorFloat,
		eTokenType::ConstructorDouble
};

Token::Token(const Token&) = default;
Token &Token::operator=(const Token&) = default;
Token::~Token() = default;

Token::Token(): _value(""), _type(eTokenType::RawValue) {}
Token::Token(std::string value) : _value(std::move(value)), _type(eTokenType::RawValue) {}
Token::Token(eTokenType type) : _value(""), _type(type) {}

const std::string &Token::getValue() const {
	return _value;
}

Token::eTokenType Token::getType() const {
	return _type;
}

