#include "ParticleFountain.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 64

//default constructor
ParticleFountain::ParticleFountain(){}

//constructor
ParticleFountain::ParticleFountain(glm::vec3 startPos, AssetShader shader, GLuint texture)
	: startPos(startPos), shader(shader), texture(texture)
{
	srand(time(NULL)); //seed the random number generator as soon as the object is created
	//add the first particle
	addParticle(0);
	pIndex = 1, frameDelay = 2;
	VAO = prepareDraw();
}

//update funtion
void ParticleFountain::update(particle &p)
{
	p.speed.y -= 0.1f; //effect of gravity
	p.pos += p.speed;
	p.pos.z -= 0.0001f;
	p.life--;

	sortParticles(particles, 100);
}

//draw function - called in main.cpp
void ParticleFountain::draw(glm::mat4 &projection, glm::mat4 &view)
{
	//the amount of particles drawn is controlled with pIndex (the number of particles currently created)
	for (int i = 0; i < pIndex; i++)
	{
		//update before drawing
		update(particles[i]);

		if (particles[i].life > 0)
		{
			shader.use();

			shader.setVec4f("customColour", particles[i].colour);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(shader.program, "myTexture"), 0);

			shader.setMat4("projection", projection);
			shader.setMat4("view", view);

			models[i] = glm::scale(models[i], glm::vec3(0.05f, 0.05f, 0.05f));
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
	if (pIndex < 100)
	{
		//frameDelay determines how long to wait before adding another particle
		//allows for a continuous flow of particles
		if (frameDelay == 0)
		{
			addParticle(pIndex);
			pIndex++;
			frameDelay = 10;
		}
	}
	frameDelay--;
}

//function called whenever a particle is to be added
void ParticleFountain::addParticle(int index)
{
	particle pTemp;
	pTemp.pos = startPos;

	glm::vec3 baseSpeed = glm::vec3(0.0075f, 2.0f, 0.0f);
	//random increase to the x and y components of the speed
	GLfloat xSpeedOffset = (GLfloat)(rand() % 100) / 1000;
	GLfloat ySpeedOffset = (GLfloat)(rand() % 100) / 1000;

	//depending on the index xSpeedOffset is subtracted instead of added
	if (index % 2 == 0)
	{
		pTemp.speed = baseSpeed;
		pTemp.speed.x += xSpeedOffset;
		pTemp.speed.y += ySpeedOffset;
	}
	else
	{
		pTemp.speed = baseSpeed;
		pTemp.speed.x *= -1;
		pTemp.speed.x -= xSpeedOffset;
		pTemp.speed.y += ySpeedOffset;
	}
	pTemp.startSpeed = pTemp.speed;
	pTemp.life = 45;
	//generating blue colour values because the effect being simulated is a water fountain
	generateBlueWithRandomOffset(pTemp);
	//add pTemp to the particles array
	particles[index] = pTemp;

	//corresponding model matrix is added
	glm::mat4 mTemp;
	models[index] = mTemp;
}

//reset a particle after it has died
//resets the position, speed, colour, and life
void ParticleFountain::resetParticle(particle &p)
{
	p.pos = startPos;
	p.speed = p.startSpeed;
	generateBlueWithRandomOffset(p);
	p.life = 45;
}