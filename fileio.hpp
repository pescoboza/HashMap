// Pedro Escoboza
// A01251531
// 14/04/2020
// TC1030.1


#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace fio {
	std::vector<std::string> readLines(const char* t_filename, unsigned t_numLines = 0U);

	std::stringstream readFile(const char* t_filename);
};

#endif // !FILE_IO_HPP