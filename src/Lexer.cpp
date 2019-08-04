/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 23:36:42 by abutok            #+#    #+#             */
/*   Updated: 2019/08/04 15:07:11 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

std::map<std::string, Lexer::eType> Lexer::_nonValueTokens;

std::vector<Token *> *Lexer::tokenize(const std::string &row) {
	_initializeNVT();
	auto *result = new std::vector<Token *>();
	for (size_t pos = 0; pos < row.size(); pos++){
		auto *token = _getNonValueToken(row, pos);
		if (token != nullptr) {
			if (token->getType() == eType::CommentBegin) {
				delete token;
				break;
			}
			result->push_back(token);
			continue;
		}
		token = _getValueToken(row, pos);
		result->push_back(token);
	}
	_trimTokenVector(*result);
	_concatRaw(*result);
	return result;
}

Token *Lexer::_getNonValueToken(const std::string &row, size_t &pos) {
	for (auto &token: _nonValueTokens) {
		if (row.find(token.first, pos) == pos) {
			pos += token.first.size() - 1;
			return new Token(token.second);
		}
	}
	return nullptr;
}

Token *Lexer::_getValueToken(const std::string &row, size_t &pos) {
	for (size_t i = pos + 1; i < row.size(); i++) {
		for (auto &token: _nonValueTokens) {
			if (row.find(token.first, i) == i) {
				auto val = row.substr(pos, i - pos);
				auto result = new Token(val);
				pos = i - 1;
				return result;
			}
		}
	}
	std::string val = row.substr(pos, row.size());
	return new Token(val);
}

void Lexer::_trimTokenVector(std::vector<Token *> &vector) {
	if (vector.empty())
		return;
	while (!vector.empty() && vector[vector.size() - 1]->getType() == eType::WS) {
		delete vector[vector.size() - 1];
		vector.pop_back();
	}
	while (!vector.empty() && (*vector.begin())->getType() == eType::WS) {
		delete *vector.begin();
		vector.erase(vector.begin());
	}
	for(auto iter = vector.begin(); iter != vector.end(); iter++) {
		if (iter + 1 != vector.end()) {
			auto t1 = *iter,
				t2 = *(iter + 1);
			while (t1->getType() == eType::WS && t2->getType() == eType::WS) {
				delete *(iter + 1);
				vector.erase(iter + 1);
				t2 = *(iter + 1);
			}
		}
	}
}

void Lexer::_concatRaw(std::vector<Token *> &vector) {
	for(auto iter = vector.begin(); iter != vector.end(); iter++) {
		if ((*iter)->getType() == eType::WS) {
			auto l = *(iter - 1),
				r = *(iter + 1);
			if (l->getType() == eType::RawValue && l->getType() == r->getType())
			{
				iter--;
				delete *(iter + 1);
				vector.erase(iter + 1);
				auto concat_val = l->getValue() + r->getValue();
				delete *(iter + 1);
				vector.erase(iter + 1);
				*iter = new Token(concat_val);
				delete l;
			}
		}
	}
}

void Lexer::_initializeNVT() {
	if (!_nonValueTokens.empty())
		return;
	_nonValueTokens.insert(std::make_pair("push", eType::Push));
	_nonValueTokens.insert(std::make_pair("assert", eType::Assert));
	_nonValueTokens.insert(std::make_pair("pop", eType::Pop));
	_nonValueTokens.insert(std::make_pair("dump", eType::Dump));
	_nonValueTokens.insert(std::make_pair("add", eType::Add));
	_nonValueTokens.insert(std::make_pair("sub", eType::Sub));
	_nonValueTokens.insert(std::make_pair("mul", eType::Mul));
	_nonValueTokens.insert(std::make_pair("div", eType::Div));
	_nonValueTokens.insert(std::make_pair("mod", eType::Mod));
	_nonValueTokens.insert(std::make_pair("print", eType::Print));
	_nonValueTokens.insert(std::make_pair("exit", eType::Exit));
	_nonValueTokens.insert(std::make_pair("int8", eType::ConstructorInt8));
	_nonValueTokens.insert(std::make_pair("int16", eType::ConstructorInt16));
	_nonValueTokens.insert(std::make_pair("int32", eType::ConstructorInt32));
	_nonValueTokens.insert(std::make_pair("float", eType::ConstructorFloat));
	_nonValueTokens.insert(std::make_pair("double", eType::ConstructorDouble));
	_nonValueTokens.insert(std::make_pair("(", eType::OBrace));
	_nonValueTokens.insert(std::make_pair(")", eType::CBrace));
	_nonValueTokens.insert(std::make_pair(";", eType::CommentBegin));
	_nonValueTokens.insert(std::make_pair("min", eType::Min));
	_nonValueTokens.insert(std::make_pair("max", eType::Max));
	_nonValueTokens.insert(std::make_pair("pow", eType::Pow));
	_nonValueTokens.insert(std::make_pair("sqrt", eType::Sqrt));
	_nonValueTokens.insert(std::make_pair("log", eType::Log));
	_nonValueTokens.insert(std::make_pair(" ", eType::WS));
	_nonValueTokens.insert(std::make_pair("\t", eType::WS));
}
