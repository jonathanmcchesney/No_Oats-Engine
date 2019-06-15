//#include "Shader.h"
//
//#include <iostream>
// // This class was common functionality developed for ELE4014 early coursework - as a result it was a group effort
// // Jonathan McChesney - Geometry Shader Functionality
//Shader::Shader() {}
//
//Shader &Shader::use()
//{
//	glUseProgram(this->program);
//	return *this;
//}
//
//void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
//{
//	GLuint vertex, fragment, geometry;
//
//	vertex = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex, 1, &vertexSource, NULL);
//	glCompileShader(vertex);
//	checkCompileErrors(vertex, "VERTEX");
//
//	fragment = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment, 1, &fragmentSource, NULL);
//	glCompileShader(fragment);
//	checkCompileErrors(fragment, "FRAGMENT");
//
//	if (geometrySource != nullptr)
//	{
//		geometry = glCreateShader(GL_GEOMETRY_SHADER);
//		glShaderSource(geometry, 1, &geometrySource, NULL);
//		glCompileShader(geometry);
//		checkCompileErrors(geometry, "GEOMETRY");
//	}
//
//	this->program = glCreateProgram();
//	glAttachShader(this->program, vertex);
//	glAttachShader(this->program, fragment);
//	glLinkProgram(this->program);
//	checkCompileErrors(this->program, "PROGRAM");
//	glDeleteShader(vertex);
//	glDeleteShader(fragment);
//}
//
//void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform1f(glGetUniformLocation(this->program, name), value);
//}
//
//void Shader::setInteger(const GLchar *name, GLint value, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform1i(glGetUniformLocation(this->program, name), value);
//}
//
//void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform2f(glGetUniformLocation(this->program, name), x, y);
//}
//
//void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform2f(glGetUniformLocation(this->program, name), value.x, value.y);
//}
//
//void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform3f(glGetUniformLocation(this->program, name), x, y, z);
//}
//
//void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform3f(glGetUniformLocation(this->program, name), value.x, value.y, value.z);
//}
//
//void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform4f(glGetUniformLocation(this->program, name), x, y, z, w);
//}
//
//void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniform4f(glGetUniformLocation(this->program, name), value.x, value.y, value.z, value.w);
//}
//
//void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(matrix));
//}
//
//
//GLuint Shader::getModelLocation(GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	GLuint loc = glGetUniformLocation(this->program, "model");
//	return loc;
//}
//GLuint Shader::getViewLocation(GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	GLuint loc = glGetUniformLocation(this->program, "view");
//	return loc;
//}
//GLuint Shader::getProjectionLocation(GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	GLuint loc = glGetUniformLocation(this->program, "projection");
//	return loc;
//}
//GLuint Shader::getOtherLocation(const GLchar *s, GLboolean useShader)
//{
//	if (useShader)
//		this->use();
//	GLuint loc = glGetUniformLocation(this->program, s);
//	return loc;
//}
//
//
//void Shader::checkCompileErrors(GLuint object, std::string type)
//{
//	GLint success;
//	GLchar infoLog[1024];
//	if (type != "PROGRAM")
//	{
//		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
//		if (!success)
//		{
//			glGetShaderInfoLog(object, 1024, NULL, infoLog);
//			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
//				<< infoLog << "\n -- --------------------------------------------------- -- "
//				<< std::endl;
//		}
//	}
//	else
//	{
//		glGetProgramiv(object, GL_LINK_STATUS, &success);
//		if (!success)
//		{
//			glGetProgramInfoLog(object, 1024, NULL, infoLog);
//			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
//				<< infoLog << "\n -- --------------------------------------------------- -- "
//				<< std::endl;
//		}
//	}
//}
//
///*
//Shader::Shader(string name)
//{
//this->name = name;
//}
//
//Shader &Shader::use()
//{
//glUseProgram(this->program);
//return *this;
//}
//
//void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource)
//{
//GLuint vertex, fragment;
//
//vertex = glCreateShader(GL_VERTEX_SHADER);
//glShaderSource(vertex, 1, &vertexSource, NULL);
//glCompileShader(vertex);
//checkCompileErrors(vertex, "VERTEX");
//
//fragment = glCreateShader(GL_FRAGMENT_SHADER);
//glShaderSource(fragment, 1, &fragmentSource, NULL);
//glCompileShader(fragment);
//checkCompileErrors(fragment, "FRAGMENT");
//
//this->program = glCreateProgram();
//glAttachShader(this->program, vertex);
//glAttachShader(this->program, fragment);
//glLinkProgram(this->program);
//checkCompileErrors(this->program, "PROGRAM");
//glDeleteShader(vertex);
//glDeleteShader(fragment);
//}
//
//void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform1f(glGetUniformLocation(this->program, name), value);
//}
//
//void Shader::setInteger(const GLchar *name, GLint value, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform1i(glGetUniformLocation(this->program, name), value);
//}
//
//void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform2f(glGetUniformLocation(this->program, name), x, y);
//}
//
//void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform2f(glGetUniformLocation(this->program, name), value.x, value.y);
//}
//
//void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform3f(glGetUniformLocation(this->program, name), x, y, z);
//}
//
//void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform3f(glGetUniformLocation(this->program, name), value.x, value.y, value.z);
//}
//
//void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform4f(glGetUniformLocation(this->program, name), x, y, z, w);
//}
//
//void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniform4f(glGetUniformLocation(this->program, name), value.x, value.y, value.z, value.w);
//}
//
//void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
//{
//if (useShader)
//this->use();
//glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(matrix));
//}
//
//
//void Shader::checkCompileErrors(GLuint object, std::string type)
//{
//GLint success;
//GLchar infoLog[1024];
//if (type != "PROGRAM")
//{
//glGetShaderiv(object, GL_COMPILE_STATUS, &success);
//if (!success)
//{
//glGetShaderInfoLog(object, 1024, NULL, infoLog);
//std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
//<< infoLog << "\n -- --------------------------------------------------- -- "
//<< std::endl;
//}
//}
//else
//{
//glGetProgramiv(object, GL_LINK_STATUS, &success);
//if (!success)
//{
//glGetProgramInfoLog(object, 1024, NULL, infoLog);
//std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
//<< infoLog << "\n -- --------------------------------------------------- -- "
//<< std::endl;
//}
//}
//}
//*/