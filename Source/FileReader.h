#ifndef SHADERPROJECT_FILEREADER_H_
#define SHADERPROJECT_FILEREADER_H_

#include <string>
#include <iostream>
#include <fstream>

class FileReader
{
public:
	FileReader(const char* filename);
	std::string ReadFileAll();
	std::string ReadFileLine();
private:
	const char* file;
	std::ifstream stream;
	int lastPosRead;

	// Helper functions
	bool FileNameExists();
	void Error(int errorcode);


};

#endif