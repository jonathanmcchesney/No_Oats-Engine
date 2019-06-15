#ifndef PARTICLE_BASE_H
#define PARTICLE_BASE_H
#include <algorithm>

//Created by Erin Hughes
//Erin Hughes LOC ~= 49

/*This header contains the particle struct and several functions that are common to all the particle objects*/
struct particle
{
	glm::vec3 pos, speed, startSpeed;
	glm::vec4 colour;
	GLfloat alpha;
	int life;

	bool operator <(particle& that)
	{
		return this->pos.z < that.pos.z;
	}
};

//wrapper method for std::sort (sorts the particles according to their position on the z axis)
static void sortParticles(particle ps[], int size)
{
	std::sort(&ps[0], &ps[size]);
}

//sets up the vertices and VAO used for drawing the particles
GLuint prepareDraw()
{
	//vertices for a 2D quad
	GLfloat tempVertices[] =
	{
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, //bottom left
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,	//bottom right
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	//top left
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f	//top right
	};

	GLuint tempVBO, tempVAO;
	glGenVertexArrays(1, &tempVAO);
	glGenBuffers(1, &tempVBO);

	glBindVertexArray(tempVAO);

	glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tempVertices), tempVertices, GL_STREAM_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return tempVAO;
}

//assigns a completely random colour to a given particle
void generateRandomColour(particle &p)
{
								 //R   //G   //B   //A
	glm::vec4 colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	colour.r = (GLfloat)(rand() % 100) / 100;
	colour.g = (GLfloat)(rand() % 100) / 100;
	colour.b = (GLfloat)(rand() % 100) / 100;
	p.colour = colour;
}

//assigns a colour with a full red value (1.0f) and random green and blue values to a given particle
void generateRedWithRandomOffset(particle &p)
{
								 //R   //G   //B   //A
	glm::vec4 colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	colour.g = (GLfloat)(rand() % 100) / 200;
	colour.b = (GLfloat)(rand() % 100) / 200;
	p.colour = colour;
}

//assigns a colour with a full green value (1.0f) and random red and blue values to a given particle
void generateGreenWithRandomOffset(particle &p)
{
								 //R   //G   //B   //A
	glm::vec4 colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	colour.r = (GLfloat)(rand() % 100) / 200;
	colour.b = (GLfloat)(rand() % 100) / 200;
	p.colour = colour;
}

//assigns a colour with a full blue value (1.0f) and random red and green values to a given particle
void generateBlueWithRandomOffset(particle &p)
{
								 //R   //G   //B   //A
	glm::vec4 colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	colour.r = (GLfloat)(rand() % 100) / 200;
	colour.g = (GLfloat)(rand() % 100) / 200;
	p.colour = colour;
}
#endif // !PARTICLE_BASE_H
