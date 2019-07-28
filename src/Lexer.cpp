/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 23:36:42 by abutok            #+#    #+#             */
/*   Updated: 2019/07/28 18:41:42 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

std::map<std::string, Lexer::type> Lexer::_nonValueTokens {

};

std::vector<Token *> *Lexer::tokenize(const std::string &row) {
	_initializeNVT();
	auto *result = new std::vector<Token *>();
	for (size_t pos = 0; pos < row.size(); pos++){
		auto *token = _getNonValueToken(row, pos);
		if (token != nullptr) {
			if (token->getType() == type::CommentBegin) {
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
				std::string val = row.substr(pos, i - pos);
				auto *result = new Token(val);
				pos = i - 1;
				return result;
			}
		}
	}
	std::string val = row.substr(pos, row.size());
	return new Token(val);
}

void Lexer::_trimTokenVector(std::vector<Token *> &vector) {
	while (vector[vector.size() - 1]->getType() == type::WS) {
		delete vector[vector.size() - 1];
		vector.pop_back();
	}
	while ((*vector.begin())->getType() == type::WS) {
		delete *vector.begin();
		vector.erase(vector.begin());
	}
	for(auto iter = vector.begin(); iter != vector.end(); iter++) {
		if (iter + 1 != vector.end()) {
			Token *t1 = *iter,
				*t2 = *(iter + 1);
			while (t1->getType() == type::WS && t2->getType() == type::WS) {
				delete *(iter + 1);
				vector.erase(iter + 1);
				t2 = *(iter + 1);
			}
		}
	}
}

void Lexer::_concatRaw(std::vector<Token *> &vector) {
	for(auto iter = vector.begin(); iter != vector.end(); iter++) {
		if ((*iter)->getType() == type::WS) {
			Token *l = *(iter - 1),
				*r = *(iter + 1);
			if (l->getType() == type::RawValue && l->getType() == r->getType())
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
	_nonValueTokens.insert(std::make_pair("push", type::Push));
	_nonValueTokens.insert(std::make_pair("assert", type::Assert));
	_nonValueTokens.insert(std::make_pair("pop", type::Pop));
	_nonValueTokens.insert(std::make_pair("dump", type::Dump));
	_nonValueTokens.insert(std::make_pair("add", type::Add));
	_nonValueTokens.insert(std::make_pair("sub", type::Sub));
	_nonValueTokens.insert(std::make_pair("mul", type::Mul));
	_nonValueTokens.insert(std::make_pair("div", type::Div));
	_nonValueTokens.insert(std::make_pair("mod", type::Mod));
	_nonValueTokens.insert(std::make_pair("print", type::Print));
	_nonValueTokens.insert(std::make_pair("exit", type::Exit));
	_nonValueTokens.insert(std::make_pair("Int8", type::ConstructorInt8));
	_nonValueTokens.insert(std::make_pair("Int16", type::ConstructorInt16));
	_nonValueTokens.insert(std::make_pair("Int32", type::ConstructorInt32));
	_nonValueTokens.insert(std::make_pair("Float", type::ConstructorFloat));
	_nonValueTokens.insert(std::make_pair("Double", type::ConstructorDouble));
	_nonValueTokens.insert(std::make_pair("(", type::OBrace));
	_nonValueTokens.insert(std::make_pair(")", type::CBrace));
	_nonValueTokens.insert(std::make_pair(";", type::CommentBegin));
	_nonValueTokens.insert(std::make_pair(" ", type::WS));
	_nonValueTokens.insert(std::make_pair("\t", type::WS));
}
