#include "AssetManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include "SOIL2\SOIL2.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 100

//hashmaps to hold all loaded assets
std::map<std::string, AssetShader> AssetManager::shaders;
std::map<std::string, GLuint> AssetManager::textures;
std::map<std::string, GLuint> AssetManager::cubemaps;

//this public function calls the corresponding private function and loads the shader from the given files
//the shader can then be accessed by the name string given
AssetShader AssetManager::addShader(const GLchar *vShader, const GLchar *fShader, std::string name)
{
	shaders[name] = loadShaderFromFile(vShader, fShader);
	return shaders[name];
}
//this function is to be used when the shader to be loaded needs a geometry shader file
AssetShader AssetManager::addGeometryShader(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader, std::string name)
{
	shaders[name] = loadGeometryShaderFromFile(vShader, fShader, gShader);
	return shaders[name];
}
//getter for shaders (works for shaders both with and without a geometry shader attached)
AssetShader AssetManager::getShader(std::string name)
{
	return shaders[name];
}


//this public function calls the corresponding private function and loads the image from the given file 
//the texture can then be accessed by the name string given
GLuint AssetManager::addTexture(const GLchar *file, std::string name)
{
	textures[name] = loadTextureFromFile(file);
	return textures[name];
}
//getter for textures
GLuint AssetManager::getTexture(std::string name)
{
	return textures[name];
}

//this public function calls the corresponding private function and loads the cubemap from the given vector of files 
//the cubemap can then be accessed by the name string given
GLuint AssetManager::addCubemap(std::vector<const GLchar * > faces, std::string name)
{
	cubemaps[name] = loadCubemapFromFile(faces);
	return cubemaps[name];
}
//getter for cubemaps
GLuint AssetManager::getCubemap(std::string name)
{
	return cubemaps[name];
}


//private function for loading in a shader with no geometry shader
//this function is private to ensure a shader cannot be loaded without attaching a name or adding it to the shaders hashmap
AssetShader AssetManager::loadShaderFromFile(const GLchar *vShader, const GLchar *fShader)
{
	std::string vertexCode;
	std::string fragmentCode;

	//attempt to read shaders files and store them in strings
	try
	{
		std::ifstream vertexShaderFile(vShader);
		std::ifstream fragmentShaderFile(fShader);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::exception e)
	{
		Logging::logError("SHADER: Failed to read shader files.",__FILE__,__LINE__);
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();

	//compile the shader with the loaded code and return it
	AssetShader shader;
	shader.compileShader(vShaderCode, fShaderCode);
	return shader;
}

//private function for loading in a shader with a geometry shader
//this function is private to ensure a shader cannot be loaded without attaching a name or adding it to the shaders hashmap
AssetShader AssetManager::loadGeometryShaderFromFile(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	//attempt to read shaders files and store them in strings
	try
	{
		std::ifstream vertexShaderFile(vShader);
		std::ifstream fragmentShaderFile(fShader);
		std::ifstream geometryShaderFile(gShader);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		gShaderStream << geometryShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();
		geometryShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::exception e)
	{
		Logging::logError("Failed to read shader files.", __FILE__, __LINE__);
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	//compile the shader with the loaded code and return it
	AssetShader shader;
	shader.compileShader(vShaderCode, fShaderCode, gShaderCode);
	return shader;
}


//private function for loading in a texture - essentially generating a GLuint corresponding to the given image and storing it
//this function is private to ensure a texture cannot be loaded without attaching a name or adding it to the textures hashmap
GLuint AssetManager::loadTextureFromFile(const GLchar *file)
{
	//generate id, load texture data
	GLuint textureID;
	glGenTextures(1, &textureID);

	int iWidth, iHeight;

	//loading in the images using SOIL2
	unsigned char *image = SOIL_load_image(file, &iWidth, &iHeight, 0,
		SOIL_LOAD_RGBA);

	//assign texture to the id
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	//texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);

	//return the generated texture id
	return textureID;
}


//private function for loading in a cubemap - essentially generating a GLuint corresponding to the given images and storing it
//this function is private to ensure a cubemap cannot be loaded without attaching a name or adding it to the textures hashmap
GLuint AssetManager::loadCubemapFromFile(std::vector<const GLchar *> faces)
{
	//generate id, load texture data
	GLuint cubemapID;
	glGenTextures(1, &cubemapID);

	int iWidth, iHeight;
	unsigned char *image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	//loading in the images using SOIL2
	//iterating through the given vector and loading each image
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &iWidth, &iHeight, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
			iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	//texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//return the generated texture id
	return cubemapID;
}

