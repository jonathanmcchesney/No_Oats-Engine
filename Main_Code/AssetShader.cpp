#include "AssetShader.h"
#include <iostream>

//Created by Erin Hughes
//Erin Hughes LOC ~= 100

//empty constructor
AssetShader::AssetShader() {}

//wrapper function to use a particular program
AssetShader &AssetShader::use()
{
	glUseProgram(this->program);
	return *this;
}

//function for compiling a shader given sources for the vertex, fragment, and geometry shaders
void AssetShader::compileShader(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
	GLuint vertex, fragment, geometry;

	//create + compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	//create + compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	//create + compile geometry shader is one is being loaded in
	if (geometrySource != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometrySource, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	//attach the vertex + fragment shaders (and the geometry shader if one is present) to the program and link it
	this->program = glCreateProgram();
	glAttachShader(this->program, vertex);
	glAttachShader(this->program, fragment);
	if (geometrySource != nullptr)
		glAttachShader(this->program, geometry);
	glLinkProgram(this->program);
	checkCompileErrors(this->program, "PROGRAM");

	//delete shaders once they have been linked
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometrySource != nullptr)
		glDeleteShader(geometry);
}

//wrapper function for setting a uniform float value in this shader
void AssetShader::setFloat(const GLchar *name, GLfloat value, GLboolean use)
{
	if (use)
		this->use();
	glUniform1f(glGetUniformLocation(this->program, name), value);
}

//wrapper function for setting a uniform integer value in this shader
void AssetShader::setInt(const GLchar *name, GLint value, GLboolean use)
{
	if (use)
		this->use();
	glUniform1i(glGetUniformLocation(this->program, name), value);
}

//wrapper function for setting a uniform vec2 in this shader (using two float values)
void AssetShader::setVec2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean use)
{
	if (use)
		this->use();
	glUniform2f(glGetUniformLocation(this->program, name), x, y);
}

//wrapper function for setting a uniform vec3 in this shader (using a vec2)
void AssetShader::setVec2f(const GLchar *name, const glm::vec2 &value, GLboolean use)
{
	if (use)
		this->use();
	glUniform2f(glGetUniformLocation(this->program, name), value.x, value.y);
}

//wrapper function for setting a uniform vec3 in this shader (using three float values)
void AssetShader::setVec3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean use)
{
	if (use)
		this->use();
	glUniform3f(glGetUniformLocation(this->program, name), x, y, z);
}

//wrapper function for setting a uniform vec4 in this shader (using a vec3)
void AssetShader::setVec3f(const GLchar *name, const glm::vec3 &value, GLboolean use)
{
	if (use)
		this->use();
	glUniform3f(glGetUniformLocation(this->program, name), value.x, value.y, value.z);
}

//wrapper function for setting a uniform vec4 in this shader (using four float values)
void AssetShader::setVec4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean use)
{
	if (use)
		this->use();
	glUniform4f(glGetUniformLocation(this->program, name), x, y, z, w);
}

//wrapper function for setting a uniform vec4 in this shader (using a vec4)
void AssetShader::setVec4f(const GLchar *name, const glm::vec4 &value, GLboolean use)
{
	if (use)
		this->use();
	glUniform4f(glGetUniformLocation(this->program, name), value.x, value.y, value.z, value.w);
}

//wrapper function for setting a uniform mat4 in this shader
void AssetShader::setMat4(const GLchar *name, const glm::mat4 &matrix, GLboolean use)
{
	if (use)
		this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

//wrapper function that returns a GLuint corresponding to the location of "model" in this shader
GLuint AssetShader::getModelLocation(GLboolean use)
{
	if (use)
		this->use();
	GLuint loc = glGetUniformLocation(this->program, "model");
	return loc;
}

//wrapper function that returns a GLuint corresponding to the location of "view" in this shader
GLuint AssetShader::getViewLocation(GLboolean use)
{
	if (use)
		this->use();
	GLuint loc = glGetUniformLocation(this->program, "view");
	return loc;
}

//wrapper function that returns a GLuint corresponding to the location of "projection" in this shader
GLuint AssetShader::getProjectionLocation(GLboolean use)
{
	if (use)
		this->use();
	GLuint loc = glGetUniformLocation(this->program, "projection");
	return loc;
}

//wrapper function that returns a GLuint corresponding to the location of the given value in this shader
GLuint AssetShader::getOtherLocation(const GLchar *name, GLboolean use)
{
	if (use)
		this->use();
	GLuint loc = glGetUniformLocation(this->program, name);
	return loc;
}

//debugging function: outputs an appropriate message if the shader fails to compile
void AssetShader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			//error message is output using the Logging class
			Logging::logErrorWithInfolog(("Shader compile-time error: Type: " + type + ".").c_str(), 
				__FILE__, __LINE__, infoLog);
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			//error message is output using the Logging class
			Logging::logErrorWithInfolog(("Shader link-time error: Type: " + type + ".").c_str(),
				__FILE__, __LINE__, infoLog);
		}
	}
}
