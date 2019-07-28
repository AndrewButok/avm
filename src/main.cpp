/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 13:58:33 by abutok            #+#    #+#             */
/*   Updated: 2019/07/26 23:49:21 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "Lexer.hpp"
#include "Executor.hpp"

int main() {
	std::string s("abcd dlp\t");
	Lexer::tokenize(s);
	return 0;
}