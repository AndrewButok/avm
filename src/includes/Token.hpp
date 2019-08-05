/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 14:08:37 by abutok            #+#    #+#             */
/*   Updated: 2019/08/05 12:28:19 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_TOKEN_HPP
#define AVM_TOKEN_HPP


#include <string>
#include <map>
#include <set>

class Token {
public:
	enum class eTokenType: unsigned int;
private:
	std::string _value;
	eTokenType _type;

	Token();
public:
	static std::map<std::string, eTokenType> _nonValueTokens;
	static std::set<eTokenType> _operators;
	static std::set<eTokenType> _constructors;

	explicit Token(std::string value);
	explicit Token(eTokenType type);
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
		WS = 12,
		ConstructorInt8 = 13,
		ConstructorInt16 = 14,
		ConstructorInt32 = 15,
		ConstructorFloat = 16,
		ConstructorDouble = 17,
		OBrace = 19,
		CBrace = 20,
		CommentBegin = 21,
		Min = 22,
		Max = 23,
		Pow = 24,
		Sqrt = 25,
		Log = 26
	};
};

#endif //AVM_TOKEN_HPP
