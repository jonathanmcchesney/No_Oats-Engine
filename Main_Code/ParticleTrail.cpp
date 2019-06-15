#include "ParticleTrail.h"

//Created by Erin Hughes
//LOC ~= 63

//default constructor
ParticleTrail::ParticleTrail(){}

//constructor
ParticleTrail::ParticleTrail(glm::vec3 startPos, AssetShader shader, GLuint texture)
	: startPos(startPos), shader(shader), texture(texture) 
{
	srand(time(NULL)); //seed the random number generator as soon as the object is created
	//add the first particle
	addParticle(0);
	pIndex = 1, frameDelay = 3;
	VAO = prepareDraw();
}

//setter for the startPos (in main.cpp this is the mousePos - allows for the particles to follow the cursor)
void ParticleTrail::setStartPos(glm::vec3 startPos)
{
	this->startPos = startPos;
}

//update function
void ParticleTrail::update(particle &p)
{
	p.speed.y += 0.00015f; //effect of gravity (added here instead of subtracted so the particles float slowly upwards)
	p.pos += p.speed;
	p.pos.z -= 0.0001f;
	p.colour.a -= 0.005f;
	p.life--;

	sortParticles(particles, 100);
}

//draw function - called in main.cpp
void ParticleTrail::draw(glm::mat4 &projection, glm::mat4 &view)
{
	//the amount of particles drawn is controlled with pIndex (the number of particles currently created)
	for (int i = 0; i < pIndex; i++)
	{
		//update before drawing
		update(particles[i]);

		if (particles[i].life > 0)
		{
			//no matrix transformation because there is no camera transformation when a trail is drawn

			shader.use();
			
			shader.setVec4f("customColour", particles[i].colour);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(shader.program, "myTexture"), 0);

			shader.setMat4("projection", projection);
			shader.setMat4("view", view);

			models[i] = glm::scale(models[i], glm::vec3(0.25f, 0.25f, 0.25f));
			models[i] = glm::translate(models[i], particles[i].pos);

			shader.setMat4("model", models[i]);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			//this line resets each model matrix (the last matrix in models[] is blank)
			models[i] = models[100];
		}
		else if (particles[i].life <= 0)
		{
			//reset the particle if it's dead (life = 0)
			resetParticle(particles[i]);
		}
	}
	//add more particles if the maximun (particleNumber) has not yet been reached
	if (pIndex < 100)
	{
		//frameDelay determines how long to wait before adding another particle
		//allows for a continuous flow of particles
		if (frameDelay == 0)
		{
			addParticle(pIndex);
			pIndex++;
			frameDelay = 3;
		}
	}
	frameDelay--;
}

//function called whenever a particle is to be added
void ParticleTrail::addParticle(int index)
{
	particle pTemp;
	pTemp.pos = startPos;

	glm::vec3 baseSpeed = glm::vec3(0.00025f, 0.00f, 0.0f);

	//random increase to the x component of the speed
	GLfloat xSpeedOffset = (GLfloat)(rand() % 100) / 10000;

	//depending on the index xSpeedOffset is subtracted instead of added
	if (index % 2 == 0)
	{
		pTemp.speed = baseSpeed;
		pTemp.speed.x += xSpeedOffset;
	}
	else
	{
		pTemp.speed = baseSpeed;
		pTemp.speed.x *= -1;
		pTemp.speed.x -= xSpeedOffset;
	}

	pTemp.startSpeed = pTemp.speed;
	pTemp.life = 250;
	generateRandomColour(pTemp);
	//add pTemp to the particles array
	particles[index] = pTemp;

	//corresponding model matrix is added
	glm::mat4 mTemp;
	models[index] = mTemp;
}

//reset a particle after it has died
//resets the position, speed, colour, and life
void ParticleTrail::resetParticle(particle &p)
{
	p.pos = startPos;
	p.speed = p.startSpeed;
	generateRandomColour(p);
	p.life = 250;
}

