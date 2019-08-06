/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 23:36:42 by abutok            #+#    #+#             */
/*   Updated: 2019/08/06 14:09:41 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

std::vector<TokenPtr> Lexer::tokenize(const std::string &row) {
	auto result = std::vector<TokenPtr>();
	for (size_t pos = 0; pos < row.size(); pos++){
		auto token = _getNonValueToken(row, pos);
		if (token != nullptr) {
			if (token->getType() == eType::CommentBegin) {
				break;
			}
			result.push_back(std::move(token));
			continue;
		}
		token = _getValueToken(row, pos);
		result.push_back(std::move(token));
	}
	_trimTokenVector(result);
	_concatRaw(result);
	return result;
}

TokenPtr Lexer::_getNonValueToken(const std::string &row, size_t &pos) {
	for (auto &token: Token::_nonValueTokens) {
		if (row.find(token.first, pos) == pos) {
			pos += token.first.size() - 1;
			return std::make_unique<Token>(token.second);
		}
	}
	return TokenPtr();
}

TokenPtr Lexer::_getValueToken(const std::string &row, size_t &pos) {
	for (size_t i = pos + 1; i < row.size(); i++) {
		for (auto &token: Token::_nonValueTokens) {
			if (row.find(token.first, i) == i) {
				auto val = row.substr(pos, i - pos);
				pos = i - 1;
				return std::make_unique<Token>(val);
			}
		}
	}
	std::string val = row.substr(pos, row.size());
	return std::make_unique<Token>(val);
}

void Lexer::_trimTokenVector(std::vector<TokenPtr> &vector) {
	if (vector.empty())
		return;
	while (!vector.empty() && vector[vector.size() - 1]->getType() == eType::WS) {
		vector.pop_back();
	}
	while (!vector.empty() && (*vector.begin())->getType() == eType::WS) {
		vector.erase(vector.begin());
	}
	for(auto iter = vector.begin(); iter != vector.end(); iter++) {
		if (iter + 1 != vector.end()) {
			while ((*iter)->getType() == eType::WS && (*(iter + 1))->getType() == eType::WS) {
				vector.erase(iter + 1);
			}
		}
	}
}

void Lexer::_concatRaw(std::vector<TokenPtr> &vector) {
	for(auto iter = vector.begin(); iter != vector.end(); iter++) {
		if ((*iter)->getType() == eType::WS) {
			auto &l = *(iter - 1),
				&r = *(iter + 1);
			if (l->getType() == eType::RawValue && l->getType() == r->getType())
			{
				iter--;
				vector.erase(iter + 1);
				auto concat_val = l->getValue() + r->getValue();
				vector.erase(iter + 1);
				*iter = std::make_unique<Token>(concat_val);
			}
		}
	}
}
