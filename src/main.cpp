/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 13:58:33 by abutok            #+#    #+#             */
/*   Updated: 2019/08/01 15:02:32 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "Parser.hpp"
#include "AVMRuntimeError.hpp"

void parse_file(std::istream& stream, const std::string& stream_name, Parser& parser) {
	std::string row;
	int row_no = 1;
	Token::eTokenType completed_instruction = Token::eTokenType::RawValue;
	while (stream) {
		try {
			getline(stream, row);
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
	parser.cleanExecutor();
}

int main(int argc, char** argv) {
	Parser *parser = Parser::getInstance();
	if (argc == 1)
		parse_file(std::cin, "stdin", *parser);
	else
		for (int i = 1; i < argc; i++){
			if (boost::filesystem::is_regular_file(argv[i])) {
				std::ifstream stream = std::ifstream(argv[i]);
				if (errno == 0)
					parse_file(stream, argv[i], *parser);
				else
					std::cerr << argv[i] << strerror(errno) << std::endl;
				} else {
					std::cerr << "Runtime error of \"" << argv[i] << "\": Not regular file"<<std::endl;
			}
		}
	delete parser;
	system("leaks Abstract-VM");
	return 0;
}