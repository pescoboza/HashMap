// Pedro Escoboza
// A01251531
// 14/04/2020
// TC1030.1


#include "fileio.hpp"

namespace fio {
	std::vector<std::string> readLines(const char* t_filename, unsigned t_numLines){
		std::ifstream ifstream;
		ifstream.open(t_filename);
		if (!ifstream.is_open()) {
			throw std::runtime_error(std::string{ "Could not open file \"" + std::string{t_filename } +"\".\n" });
		}

		std::vector<std::string> lines;
		std::string line;
		unsigned lineCounter{ 0U };
		while (std::getline(ifstream, line)) {
			if (t_numLines != 0U && lineCounter >= t_numLines) { break; }
			lines.emplace_back(std::move(line));
		}
		ifstream.close();
		return lines;
	}

	std::stringstream readFile(const char* t_filename)	{
		std::ifstream ifstream{ t_filename };
		if (!ifstream.is_open()) {
			std::string error{"Could not open file \"" + std::string{t_filename } +"\".\n" };
			std::cerr << error;
			throw std::runtime_error(error);
		}

		std::stringstream stream;
		if (ifstream) {
			stream << ifstream.rdbuf();
		}
		ifstream.close();

		return stream;
	}
}