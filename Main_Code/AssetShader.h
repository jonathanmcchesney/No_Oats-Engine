#ifndef ASSET_SHADER_H
#define ASSET_SHADER_H

/*shader object that works with the asset manager*/

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include "Logging.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 20

class AssetShader
{
public:

	//variable for the shader project
	GLuint program;
	
	//empty constructor
	AssetShader();

	AssetShader &use();

	//compile function
	void compileShader(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);

	//wrapper function for OpenGL functionality
	void setFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void setInt(const GLchar *name, GLint value, GLboolean useShader = false);
	void setVec2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void setVec2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void setVec3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void setVec3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void setVec4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void setVec4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void setMat4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
	GLuint getModelLocation(GLboolean useShader = false);
	GLuint getViewLocation(GLboolean useShader = false);
	GLuint getProjectionLocation(GLboolean useShader = false);
	GLuint getOtherLocation(const GLchar *s, GLboolean useShader = false);

private:

	//function that checks for errors
	void checkCompileErrors(GLuint object, std::string type);
};
#endif // !ASSET_SHADER_H