#ifndef H_FILE_READER
#define H_FILE_READER

#include <iostream>
#include <fstream>
#include <string>

class File_Reader 
{
public:
	File_Reader() {};
	~File_Reader() {};
	std::vector<std::string> Read_File(const std::string fileName);
};

#endif