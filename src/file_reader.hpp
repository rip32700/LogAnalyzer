#ifndef H_FILE_READER
#define H_FILE_READER

#include <iostream>
#include <fstream>
#include <string>

/**
  * @author: prieger
  * File Reader object
  * - used to read in a file of given name and returning its lines 
  *   as a vector
  */

class File_Reader 
{
public:

	/* CONSTRUCTOR and DESTRUCTOR */
	File_Reader() {};

	~File_Reader() {};

	/* FUNCTIONS */

	inline std::vector<std::string> Read_File(const std::string fileName) {

		std::ifstream infile(fileName);
		std::vector<std::string> lines;

		// read file
		std::string line;
		while (std::getline(infile, line)) {
			lines.push_back(line);
		}

		return lines;
	}
};

#endif