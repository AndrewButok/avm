/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 13:58:33 by abutok            #+#    #+#             */
/*   Updated: 2019/07/31 15:09:12 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Parser.hpp>
#include <iostream>
#include <fstream>
#include "AVMRuntimeError.hpp"

void parse_file(std::istream& stream, const std::string& stream_name, Parser& parser) {
	std::string row;
	int row_no = 1;
	Token::eTokenType completed_instruction = Token::eTokenType::RawValue;
	while (getline(stream, row).good()) {
		try {
			if (row.empty()) {
				row_no++;
				continue;
			}
			completed_instruction = parser.parse(row);
			if (completed_instruction == Token::eTokenType::Exit)
				break;
			if (stream.eof())
				throw AVMRuntimeError("No Exit instruction in file.");
		} catch (AVMRuntimeError &ex) {
			std::cerr << "Runtime error of \"" << stream_name << "\" at line " << row_no << ": " << ex.what() << std::endl;
		}
		row_no++;
	}
	if (completed_instruction != Token::eTokenType::Exit)
        std::cerr << "Runtime error of \"" << stream_name << "\" at line " << row_no <<
            ": No exit instruction at the end fo the file" << std::endl;
}

int main(int argc, char** argv) {
	Parser *parser = Parser::getInstance();
	if (argc == 1)
		parse_file(std::cin, "stdin", *parser);
	else
		for (int i = 1; i < argc; i++){
			std::ifstream stream = std::ifstream(argv[i]);
			parse_file(stream, argv[i], *parser);
		}
	delete parser;
	system("leaks Abstract-VM");
	return 0;
}