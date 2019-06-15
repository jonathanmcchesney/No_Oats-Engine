#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MAGAGER_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>
#include "AssetShader.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 18

class AssetManager
{
public:
	//hashmaps to hold all loaded assets
	static std::map<std::string, AssetShader> shaders;
	static std::map<std::string, GLuint> textures;
	static std::map<std::string, GLuint> cubemaps;

	//public functions for loading a shader or geometry shader
	static AssetShader addShader(const GLchar *vShader, const GLchar *fShader, std::string name);
	static AssetShader addGeometryShader(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader, std::string name);
	//getter for a given shader
	static AssetShader getShader(std::string name);

	//public function for loading a texture
	static GLuint addTexture(const GLchar *file, std::string name);
	//getter for a given shader
	static GLuint getTexture(std::string name);

	//public function for loading a cubemap
	static  GLuint addCubemap(std::vector<const GLchar * > faces, std::string name);
	//getter for a given cubemap
	static  GLuint getCubemap(std::string name);

private:
	//private constructor and function
	AssetManager() {}
	static AssetShader loadShaderFromFile(const GLchar *vShader, const GLchar *fShader);
	static AssetShader loadGeometryShaderFromFile(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader);
	static GLuint loadTextureFromFile(const GLchar *file);
	static GLuint loadCubemapFromFile(std::vector<const GLchar * > faces);
};
#endif