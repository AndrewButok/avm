/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:24:21 by abutok            #+#    #+#             */
/*   Updated: 2019/07/31 22:03:23 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "Token.hpp"

Token::Token(): _value(""), _type(eTokenType::RawValue) {}
Token::Token(std::string value) : _value(std::move(value)), _type(eTokenType::RawValue) {}
Token::Token(eTokenType type) : _value(""), _type(type) {}

const std::string &Token::getValue() const {
	return _value;
}

Token::eTokenType Token::getType() const {
	return _type;
}

