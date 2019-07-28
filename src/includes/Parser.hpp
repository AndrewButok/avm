/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 19:35:15 by abutok            #+#    #+#             */
/*   Updated: 2019/07/28 19:44:33 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_PARSER_HPP
#define AVM_PARSER_HPP

#include "OperandFactory.hpp"
#include "Executor.hpp"

class Parser {
private:
	static Parser *instance;

	OperandFactory	*_operandFactory;
	Executor		*_executor;

	Parser();
	Parser(const Parser&) = default;
	Parser &operator=(const Parser&) = default;
	static void _checkTokens(std::vector<Token *>&);
	static void _deleteTokens(std::vector<Token *> *);
public:
	virtual ~Parser();
	Parser *getInstance();

	void parse(const std::string &);
};


#endif //AVM_PARSER_HPP
