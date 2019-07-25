/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 14:08:37 by abutok            #+#    #+#             */
/*   Updated: 2019/07/25 17:45:03 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_TOKEN_HPP
#define AVM_TOKEN_HPP


#include <string>

class Token {
public:
	enum class eTokenType: unsigned int;
private:
	std::string _value;
	eTokenType _type;

	Token() = default;
public:
	Token(std::string &value, eTokenType type);
	Token(const Token&) = default;
	Token &operator=(const Token&) = default;
	virtual ~Token() = default;
	const std::string &getValue() const;
	eTokenType getType() const;

	enum class eTokenType: unsigned int{
		RawValue = 0,
		Push = 1,
		Assert = 2,
		Pop = 3,
		Dump = 4,
		Add = 5,
		Sub = 6,
		Mul = 7,
		Div = 8,
		Mod = 9,
		Print = 10,
		Exit = 11,
		ConstructorInt8 = 12,
		ConstructorInt16 = 13,
		ConstructorInt32 = 14,
		ConstructorFloat = 15,
		ConstructorDouble = 16,
	};
};

#endif //AVM_TOKEN_HPP
