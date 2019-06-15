#pragma once
/*collection of functions for logging out errors across the project*/

//Created by Erin Hughes
//Erin Hughes LOC ~= 25

class Logging
{
private:
	//private function for shortening the filename printed in messages
	//e.g. "InitialiseOpenGL/main.cpp" becomes "main.cpp"
	static std::string shortenFilename(std::string filename)
	{
		const size_t lastSlash = filename.find_last_of("\\/");
		if (std::string::npos != lastSlash)
		{
			filename.erase(0, lastSlash + 1);
		}
		return filename;
	}

public:
	//logging out a general message
	static void logGeneral(std::string message, std::string filename, int line)
	{
		shortenFilename(filename);
		std::cout << message << "||" << filename << " (" << line << ")" << std::endl;
	}

	//logging out a general message without a file name or line number
	static void logMessageOnly(std::string message)
	{
		std::cout << message << std::endl;
	}

	//logging out a non-specific error
	static void logError(std::string message, std::string filename, int line)
	{
		shortenFilename(filename);
		std::cout << "ERROR::" << message << "||" << filename << " (" << line << ")" << std::endl;
	}
	
	//logging out an error with an infolog (used mainly when generating shaders)
	static void logErrorWithInfolog(std::string message, std::string filename, int line, GLchar *infolog)
	{
		shortenFilename(filename);
		std::cout << "ERROR::" << message << "||" << filename << " (" << line << ")\n" << infolog << std::endl;
	}

	//logging out an error with FreeType
	static void logFreetypeError(std::string message, std::string filename, int line)
	{
		shortenFilename(filename);
		std::cout << "ERROR::FREETYPE::" << message << "||" << filename << " (" << line << ")" << std::endl;
	}

	//Logging out an error with AssImp
	static void logAssimpError(std::string message, std::string filename, int line)
	{
		shortenFilename(filename);
		std::cout << "ERROR::ASSIMP::" << message << "||" << filename << " (" << line << ")" << std::endl;
	}
};


