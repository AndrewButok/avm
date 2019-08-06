/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 16:21:06 by abutok            #+#    #+#             */
/*   Updated: 2019/08/05 12:07:34 by abutok           ###   ########.fr       */
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

	static TokenPtr _getNonValueToken(const std::string &row, size_t &pos);
	static TokenPtr _getValueToken(const std::string &row, size_t &pos);
	static void _trimTokenVector(std::vector<TokenPtr> &vector);
	static void _concatRaw(std::vector<TokenPtr> &vector);
public:
	static std::vector<TokenPtr> tokenize(const std::string &row);
};

#endif
