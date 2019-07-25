/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 14:29:17 by abutok            #+#    #+#             */
/*   Updated: 2019/07/25 17:45:03 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "Token.hpp"

Token::Token(std::string &value, Token::eTokenType type) : _value(value), _type(type) {}

const std::string &Token::getValue() const {
	return _value;
}

Token::eTokenType Token::getType() const {
	return _type;
}

