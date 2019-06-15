#include "TextureAtlas.h"

//Created by Erin Hughes
//Erin Hughes LOC ~= 72

//default constructor
TextureAtlas::TextureAtlas(){}

//constructor
TextureAtlas::TextureAtlas(GLuint texture, int numberOfRows, glm::vec3 pos, glm::vec3 scale)
	: texture(texture), numberOfRows(numberOfRows), pos(pos), scale(scale)
{
	//caluclating the animation length and VAO for this atlas
	this->animationLength = calcAnimationLength();
	this->VAO = prepareDraw();
}

//function for calculating the animation length 
//currently set to 10 x numbeOfRows but could easily be tweaked to make the animations longer/shorter
GLuint TextureAtlas::calcAnimationLength()
{
	GLuint length = numberOfRows * 10;
	return length;
}

//sets up the vertices and VAO used for drawing the atlas
GLuint TextureAtlas::prepareDraw()
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

//function for updating the offset
//the offset is used in the shader to determine the current animation being drawn
void TextureAtlas::updateOffset()
{
	//stride = value of each frame
	GLfloat stride = 1.0 / numberOfRows;
	//limit = maximum number of frames
	GLfloat limit = stride * (numberOfRows - 1);

	//update the offset after a certain amount of time has passed
	if (animationLength % (numberOfRows) == 0)
	{
		//increment the x offset until it reaches the limit, then reset to 0 and incrememnt the y offset
		currentOffset.x += stride;
		if (currentOffset.x > limit)
		{
			currentOffset.x = 0.0f;
			currentOffset.y += stride;
			//reset the offset if the y offset has reached the limit
			if (currentOffset.y > (limit))
			{
				currentOffset = glm::vec2(0.0f, 0.0f);
			}
		}
	}
	//reset the animationLength after all frames have been played
	if (animationLength <= 0)
	{
		animationLength = calcAnimationLength();
	}
}

//update function
void TextureAtlas::update()
{
	updateOffset();
	animationLength -= numberOfRows;
}

//draw function - called in main.cpp
void TextureAtlas::draw(AssetShader shader, glm::mat4 &projection, glm::mat4 &view)
{
	//update before drawing
	update();

	glm::mat4 model;

	shader.use();
	shader.setFloat("numberOfRows", numberOfRows);
	shader.setVec2f("offset", currentOffset);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader.program, "currentFrame"), 0);

	//translate the atlas to the position specified in the constructor
	model = glm::translate(model, pos);

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	//matrix transformation so the particles always face the camera
	model[0][0] = view[0][0];
	model[0][1] = view[1][0];
	model[0][2] = view[2][0];
	model[1][0] = view[0][1];
	model[1][1] = view[1][1];
	model[1][2] = view[2][1];
	model[2][0] = view[0][2];
	model[2][1] = view[1][2];
	model[2][2] = view[2][2];

	//scale the atlas using the vec3 specified in the constructor
	model = glm::scale(model, scale);

	shader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

//function to change the texture applied to the atlas
//also updates the numberOfRows and animationLength, and resets the currentOffset
void TextureAtlas::changeTexture(GLuint texture, int numberOfRows)
{
	this->texture = texture;
	this->numberOfRows = numberOfRows;
	animationLength = calcAnimationLength();
	currentOffset = glm::vec2(0.0f, 0.0f);
}




