#ifndef PARTICLE_TRAIL_H
#define PARTICLE_TRAIL_H

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <time.h>
#include <vector>
#include "AssetShader.h"
#include "ParticleBase.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 16

class ParticleTrail
{
private:
	//variables
	glm::vec3 startPos;
	AssetShader shader;
	GLuint texture, VAO;
	//array that contains all the particles to be used
	particle particles[100];
	//the models array is initialised to one larger than the particles array so that the models can all be reset each frame
	glm::mat4 models[101]; 
	int pIndex, frameDelay;

public:
	//default constructor
	ParticleTrail();

	//constructor
	ParticleTrail(glm::vec3 startPos, AssetShader shader, GLuint texture);

	//functions
	void setStartPos(glm::vec3 startPos);
	void update(particle &p);
	void draw(glm::mat4 &projection, glm::mat4 &view);
	void addParticle(int index);
	void resetParticle(particle &p);
};
#endif // !PARTICLE_TRAIL_H
