/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 13:58:33 by abutok            #+#    #+#             */
/*   Updated: 2019/08/05 11:48:50 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <boost/filesystem.hpp>
#include "Parser.hpp"
#include "AVMRuntimeError.hpp"

void parse_rows(std::vector<std::string> vector,
		const std::string& stream_name, Parser& parser) {
	if (vector.empty())
		return;
	auto completed_instruction = Token::eTokenType::RawValue;
	for (size_t i = 0; i < vector.size(); i++) {
		try {
			if (vector[i].empty())
				continue;
			completed_instruction = parser.parse(vector[i]);
			if (completed_instruction == Token::eTokenType::Exit)
				break;
		} catch (AVMRuntimeError &ex) {
			std::cerr << "Runtime error of \"" << stream_name
				<< "\" at line " << i + 1 << ": " << ex.what() << std::endl;
		}
	}
	if (completed_instruction != Token::eTokenType::Exit)
		std::cerr << "Runtime error of \"" << stream_name << "\" at line "
			<< vector.size() << ": No exit instruction at the end fo the file" << std::endl;
	parser.cleanExecutor();
}

std::vector<std::string>  parse_file(std::istream& stream) {
	auto row = std::string();
	auto file = std::vector<std::string>();
	while (stream) {
		getline(stream, row);
		file.push_back(row);
	}
	return file;
}

std::vector<std::string>  parse_cin(std::istream& stream) {
	auto row = std::string();
	auto file = std::vector<std::string>();
	while (stream) {
		getline(stream, row);
		auto eof = row.find(";;");
		if (eof == std::string::npos)
			file.push_back(row);
		else {
			file.push_back(row.substr(0, eof));
			break;
		}
	}
	return file;
}

int main(int argc, char** argv) {
	auto parser = Parser::getInstance();
	if (argc == 1) {
		auto rows = parse_cin(std::cin);
		parse_rows(rows, "stdin", *parser);
	}
	else {
		for (int i = 1; i < argc; i++) {
			if (boost::filesystem::is_regular_file(argv[i])) {
				auto stream = std::ifstream(argv[i]);
				if (errno == 0) {
					auto rows = parse_file(stream);
					parse_rows(rows, argv[i], *parser);
				} else
					std::cerr << argv[i] << strerror(errno) << std::endl;
			} else {
				std::cerr << "Runtime error of \"" << argv[i]
						  << "\": Not regular file" << std::endl;
			}
		}
	}
	delete parser;
	return 0;
}