/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 16:21:06 by abutok            #+#    #+#             */
/*   Updated: 2019/07/28 11:05:37 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_LEXER_HPP
#define AVM_LEXER_HPP

#include <vector>
#include <map>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include "Token.hpp"

namespace lex = boost::spirit::lex;

template <typename T>
class Lexer: lex::lexer<T>{
public:
	Lexer() {
		this->self.add
			("push", Token::eTokenType::Push)
			("")
			()
			()
			()
			()
			()
			()
			()
			()
			()
			()
			()
			()
			()
			()
			;
	}

};

#endif
