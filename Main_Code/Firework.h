#ifndef FIREWORK_H
#define FIREWORK_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <time.h>
#include "AssetShader.h"
#include "ParticleBase.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 15

class Firework
{
private:
	//variables
	glm::vec3 startPos;
	GLuint VAO, texture;
	AssetShader shader;
	//array that contains all the particles to be used
	particle particles[20];
	//the models array is initialised to one larger than the particles array so that the models can all be reset each frame
	glm::mat4 models[21]; 

public:
	//default constructor
	Firework();

	//constructor
	Firework(glm::vec3 startPos, AssetShader shader, GLuint texture);

	//functions
	void update(particle &p);
	void draw(glm::mat4 &projection, glm::mat4 &view);
	void generateSpeed(particle &p);
	void addParticles(int max);
	void resetParticles(int max);
};
#endif // !FIREWORK_H
