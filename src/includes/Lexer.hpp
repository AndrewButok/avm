/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 16:21:06 by abutok            #+#    #+#             */
/*   Updated: 2019/08/04 14:22:51 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_LEXER_HPP
#define AVM_LEXER_HPP

#include <vector>
#include <map>
#include "Token.hpp"

class Lexer{
private:
	typedef Token::eTokenType eType;
	static std::map<std::string, eType> _nonValueTokens;

	static Token* _getNonValueToken(const std::string &row, size_t &pos);
	static Token* _getValueToken(const std::string &row, size_t &pos);
	static void _trimTokenVector(std::vector<Token *> &vector);
	static void _concatRaw(std::vector<Token *> &vector);
	static void _initializeNVT();

public:
	static std::vector<Token *> *tokenize(const std::string &row);
};

#endif
