#ifndef SHADER_HEADER
#define SHADER_HEADER
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <GL/glew.h>

#include "Logging.h"

// This class was common functionality developed for ELE4014 early coursework - as a result it was a group effort
// Jonathan McChesney - Geometry Shader functionality

// Shader Helper class
class Shader
{
public:
	// Declaring the program
	GLuint Program;

	std::string vertPath, fragPath;

	// Shader Constructor
	Shader(const GLchar *vPath, const GLchar *fPath, const GLchar* geometryPath = nullptr)
	{
		// From filepath get the vertex source code and the fragment source code
		std::string vCode; // vertex code
		std::ifstream vertexShaderFile;
		std::string fCode; // fragment code
		std::ifstream fragmentShaderFile;
		std::string geometryCode;
		std::ifstream gShaderFile;

		vertPath = vPath;
		fragPath = fPath;

		fragmentShaderFile.exceptions(std::ifstream::badbit); // exceptions
		vertexShaderFile.exceptions(std::ifstream::badbit); // exceptions
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexShaderFile.open(vPath); // open v file
			fragmentShaderFile.open(fPath); // ppen fragment file
			std::stringstream vertexShaderStream, fragmentShaderStream;

			// Next read the buffer files contents into the relevant streams
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			vertexShaderFile.close(); // Close v handler
			fragmentShaderFile.close(); // close f handler

			vCode = vertexShaderStream.str(); // convert v into string
			fCode = fragmentShaderStream.str(); // convert f into string
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}

		catch (std::ifstream::failure e) // catch statement
		{
			//std::cout << "ERROR::SHADER::FILE_UNSUCCESFULLY_READ" << std::endl;
			Logging::logError("SHADER::FILE_UNSUCCESSFULLY_READ", __FILE__, __LINE__);
		}
		const GLchar *vShaderCode = vCode.c_str(), *fShaderCode = fCode.c_str();

		// Shaders are now compiled
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// Fragment Shader: - Show compile errors
		// Attach, compile and show errors
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			//std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			Logging::logErrorWithInfolog("SHADER::FRAGMENT::COMPILATION_FAILED", fPath, __LINE__,infoLog);
		}
		GLuint geometry;
		if (geometryPath != nullptr)
		{
			const GLchar * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(geometry, 512, NULL, infoLog);
				//std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
				Logging::logErrorWithInfolog("SHADER::GEOMETRY::COMPILATION_FAILED", geometryPath, __LINE__, infoLog);
			}
		}

		// Vertex Shader:- Show compile errors
		// Attach, compile and show errors
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			Logging::logErrorWithInfolog("SHADER::VERTEX::COMPILATION_FAILED", vPath, __LINE__, infoLog);
		}

		// Shader Program :- Show Linking errors
		// Attach the shaders, link, show errors
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		if (geometryPath != nullptr)
			glAttachShader(this->Program, geometry);
		glLinkProgram(this->Program);
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			//std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			Logging::logErrorWithInfolog("SHADER::PROGRAM::COMPILATION_FAILED", __FILE__, __LINE__, infoLog);
		}

		glDeleteShader(fragment);
		glDeleteShader(vertex); // delete Shaders
		if (geometryPath != nullptr)
			glDeleteShader(geometry);
	}

	Shader() {};

	void UseCurrentShader() // use the current shader
	{
		glUseProgram(this->Program);
	}

	std::string GetVertexPath()
	{
		return vertPath;
	}

	std::string GetFragPath()
	{
		return fragPath;
	}
};

#endif
