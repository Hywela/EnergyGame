
#include "FileReader.h"

FileReader::FileReader(const char* filename) : file(filename) {}
// Reads a file in one go
std::string FileReader::ReadFileAll()
{
	if(FileNameExists())
	{
		stream.open(file, std::ios::in | std::ios::binary);
		if (stream)
		{
			std::string contents;
			stream.seekg(0, std::ios::end);
			contents.resize(stream.tellg());
			stream.seekg(0, std::ios::beg);
			stream.read(&contents[0], contents.size());
			stream.close();
			return(contents);
		}
		return 0;
	}
	else{
		Error(1);
	}
};

// Reads a file in one go
std::string FileReader::ReadFileLine()
{
	if(FileNameExists())
	{
		if(!stream.is_open())
		{
			stream.open(file, std::ios::in | std::ios::binary);
		}
		char res[1024];
		stream.getline(res,1024);
		return res;
		
	}
	else{
		Error(1);
	}
	return nullptr;
};


// Check if file
bool FileReader::FileNameExists()
{
	return (file != "" || file != nullptr) ? true : false;
};



void FileReader::Error(int errorcode)
{
	switch(errorcode)
	{
	case 1:
		{
			std::cout << "Error reading file. Filename not set?\n";
			break;
		}
	default:
		{
			std::cout << "Unknown error was detected in FileReader. Error code was: " << errorcode << "...\n";
			break;
		}
	};
}