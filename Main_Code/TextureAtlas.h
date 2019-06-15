#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "GL\glew.h"
#include "SOIL2\SOIL2.h"
#include <string>
#include <iostream>
#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include "AssetShader.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 16

class TextureAtlas
{
private:
	//variables
	const GLchar *file;
	GLuint textureID, animationLength, VAO, texture;
	int numberOfRows;
	glm::vec2 currentOffset;
	glm::vec3 pos, scale;

	//private functions
	GLuint calcAnimationLength();
	GLuint prepareDraw();

public:
	//default constructor
	TextureAtlas();

	//constructor
	TextureAtlas(GLuint texture, int numberOfRows, glm::vec3 pos, glm::vec3 scale);

	//functions
	void updateOffset();
	void update();
	void draw(AssetShader shader, glm::mat4 &projection, glm::mat4 &view);
	void changeTexture(GLuint texture, int numberOfRows);
};
#endif // !TEXTURE_ATLAS_H
