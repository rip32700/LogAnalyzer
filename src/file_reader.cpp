#include "file_reader.hpp"

std::string File_Reader::Read_File(const std::string fileName) 
{
	std::ifstream ifs(fileName);
  	std::string content;
	content.assign( (std::istreambuf_iterator<char>(ifs) ),
                	(std::istreambuf_iterator<char>()    ) );

	return content;
}