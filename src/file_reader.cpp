#include "file_reader.hpp"

std::vector<std::string> File_Reader::Read_File(const std::string fileName) 
{
	std::ifstream infile(fileName);
	std::vector<std::string> lines;

	// read file
	std::string line;
	while (std::getline(infile, line))
	{
		lines.push_back(line);
	}

	return lines;
}