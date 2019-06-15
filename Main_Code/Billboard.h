#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Shader.h"
#include "glm.hpp"
#include "SOIL2/SOIL2.h"

/*README: Constructor takes in a glm::vec3 for the xyz position of the billboard,
a glm::vec2 for the xy Dimensions of the billboard(1, 1)=square and,
a filename, e.g. "res/images/foo.jpg".
*/
class Billboard
{
public:

	GLuint VBO, VAO, EBO, texture1;
	Shader quadShader;
	glm::vec3 quadPosition;
	glm::vec2 quadSizeXY;
	const char* fileName;

	Billboard(glm::vec3 billboardPosition, glm::vec2 size, const char* fileName) :quadShader("res/shaders/billboard.vertex", "res/shaders/billboard.fragment"),
		quadPosition(billboardPosition),
		quadSizeXY(size),
		fileName(fileName)
	{

		init();
	}


	void Draw(glm::mat4& projectionMat, Camera camera)
	{

		quadShader.UseCurrentShader();
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(quadShader.Program, "ourTexture1"), 0);
		glUniformMatrix4fv(glGetUniformLocation(quadShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix())); // view data is retrieved
		glUniformMatrix4fv(glGetUniformLocation(quadShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat)); // projection data is retrieved

		glUniform3f(glGetUniformLocation(quadShader.Program, "CameraRight_worldspace"), camera.right.x, camera.right.y, camera.right.z);
		glUniform3f(glGetUniformLocation(quadShader.Program, "CameraUp_worldspace"), camera.up.x, camera.up.y, camera.up.z);
		glUniform3f(glGetUniformLocation(quadShader.Program, "BillboardPos"), quadPosition.x, quadPosition.y, quadPosition.z);
		glUniform2f(glGetUniformLocation(quadShader.Program, "BillboardSize"), quadSizeXY.x, quadSizeXY.y);


		// Draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void init()
	{
		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat quadvertices[] = {
			// Positions          // Colors           // Texture Coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadvertices), quadvertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO


		// ====================
		// Texture 1
		// ====================
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
												// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		int qwidth, qheight;
		unsigned char* qimage = SOIL_load_image(fileName, &qwidth, &qheight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, qwidth, qheight, 0, GL_RGB, GL_UNSIGNED_BYTE, qimage);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(qimage);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


	}

private:

};
#endif
