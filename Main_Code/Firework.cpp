#include "Firework.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 84

//default constructor
Firework::Firework(){}

//constructor
Firework::Firework(glm::vec3 startPos, AssetShader shader, GLuint texture)
	: startPos(startPos), shader(shader), texture(texture)
{
	srand(time(NULL)); //seed the random number generator as soon as the object is created
	addParticles(20);
	sortParticles(particles, 20);
	VAO = prepareDraw();
}

//update funtion
void Firework::update(particle &p)
{
	p.speed.y -= 0.05f; //effect of gravity
	p.pos += p.speed;
	p.pos.z -= 0.0001f;
	p.colour.a -= 0.035f;
	p.life--;

	sortParticles(particles, 20);
}

//draw function - called in main.cpp
void Firework::draw(glm::mat4 &projection, glm::mat4 &view)
{
	for (int i = 0; i < 20; i++)
	{
		//update before drawing
		update(particles[i]);

		if (particles[i].life > 0)
		{
			//matrix transformation so the particles always face the camera
			models[i][0][0] = view[0][0];
			models[i][0][1] = view[1][0];
			models[i][0][2] = view[2][0];
			models[i][1][0] = view[0][1];
			models[i][1][1] = view[1][1];
			models[i][1][2] = view[2][1];
			models[i][2][0] = view[0][2];
			models[i][2][1] = view[1][2];
			models[i][2][2] = view[2][2];

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
			models[i] = models[20];
		}
		else if (particles[i].life <= 0)
		{
			//reset all particles if one is dead
			//because all particles are spawned at once they will have the same life value, so if one is dead they must all be dead
			resetParticles(20);
		}
	}
}

//generates random x and y offsets for the speed of a given particle
void Firework::generateSpeed(particle & p)
{
	glm::vec3 baseSpeed = glm::vec3(0.075f, 0.75f, 0.0f);
	//random increase to the x and y components of the speed
	GLfloat xSpeedOffset = (GLfloat)(rand() % 100) / 500;
	GLfloat ySpeedOffset = (GLfloat)(rand() % 200) / 1000;

	int neg = (GLint)(rand() % 2);
	//depending on the neg value generated xSpeedOffset is subtracted instead of added
	if (neg % 2 == 0)
	{
		p.speed = baseSpeed;
		p.speed.x += xSpeedOffset;
		p.speed.y += ySpeedOffset;
	}
	else
	{
		p.speed = baseSpeed;
		p.speed.x *= -1;
		p.speed.x -= xSpeedOffset;
		p.speed.y += ySpeedOffset;
	}
	p.startSpeed = p.speed;
}

//function called whenever the particles for the firework need to be added to the particles array
void Firework::addParticles(int max)
{
	//to make the fireworks appear more random the starting x position for each firework is randomly offset
	GLfloat xPosOffset = (GLfloat)(rand() % 100) / 5;
	//depending on the neg value generated xSpeedOffset is subtracted instead of added
	int neg = (int)(rand() % 2);
	if (neg > 0)
	{
		xPosOffset *= -1;
	}
	//add max particles
	for(int i = 0; i < max; i++)
	{
		particle pTemp;
		pTemp.pos = startPos;
		pTemp.pos.x = xPosOffset;
		pTemp.pos.z = (GLfloat)i / 100000;
		//generate a random speed
		generateSpeed(pTemp);
		pTemp.life = 50;
		generateRandomColour(pTemp);
		//add pTemp to the particles array
		particles[i] = pTemp;

		//corresponding model matrix is added
		glm::mat4 mTemp;
		models[i] = mTemp;
	}
}

//reset all particles after they have died
//resets the position, speed, colour, and life
void Firework::resetParticles(int max)
{
	//to make the fireworks appear more random the starting x position for each firework is randomly offset
	GLfloat xPosOffset = (GLfloat)(rand() % 100) / 5;
	//depending on the neg value generated xSpeedOffset is subtracted instead of added
	int neg = (int)(rand() % 2);
	if (neg > 0)
	{
		xPosOffset *= -1;
	}
	//reset max particles
	for (int i = 0; i < max; i++)
	{
		particles[i].pos = startPos;
		particles[i].pos.x = xPosOffset;
		generateSpeed(particles[i]);
		generateRandomColour(particles[i]);
		particles[i].life = 50;
	}
}