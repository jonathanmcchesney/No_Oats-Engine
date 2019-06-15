#include "ManualQuad.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~210

// Constructor for the Manually created quad class
ManualQuad::ManualQuad() 
{
}

// Initialise an advanced quad with verticies for the position, normal and texture coordinates (Lazy initialisation is used here).
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
// This initialisation is a bit more complex than other generation (Epic games research paper was helpful in designing this). We set up ou positions for the verts (4 as some are shared),
// Then we set up our uv texture coordinates (as well as the normal). Then we calculate the Tangent and bittangent (cross product of normal and tangent). We do this for each triangle, using the delta
// coordinates dound by the difference between the first two and last two coordinate, along with the edge position found in a similar fashion. By solving the following equation:
// [TxBxTy ByTzBz] = 1/(ΔU1ΔV2 − ΔU2ΔV1) * [ΔV2−ΔU2−ΔV1ΔU1] * [E1xE2xE1y E2yE1zE2z] we were able to find the T and B components. Making sure to normalise to convert into a unit vector.
void ManualQuad::RenderAdvancedQuad()
{
	if (advancedQuadVAO == 0)
	{
		// positions
		glm::vec3 position1(-1.0, 1.0, 0.0);
		glm::vec3 position2(-1.0, -1.0, 0.0);
		glm::vec3 position3(1.0, -1.0, 0.0);
		glm::vec3 position4(1.0, 1.0, 0.0);
		// texture coordinates
		glm::vec2 uvPosition1(0.0, 1.0);
		glm::vec2 uvPosition2(0.0, 0.0);
		glm::vec2 uvPosition3(1.0, 0.0);
		glm::vec2 uvPosition4(1.0, 1.0);
		// normal vector
		glm::vec3 normal(0.0, 0.0, 1.0);

		// calculate the tangent andbitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		glm::vec3 edgePosition1 = position2 - position1;
		glm::vec3 edgePosition2 = position3 - position1;
		glm::vec2 deltaUVPosition1 = uvPosition2 - uvPosition1;
		glm::vec2 deltaUVPosition2 = uvPosition3 - uvPosition1;

		GLfloat floater = 1.0f / (deltaUVPosition1.x * deltaUVPosition2.y - deltaUVPosition2.x * deltaUVPosition1.y);

		tangent1.x = floater * (deltaUVPosition2.y * edgePosition1.x - deltaUVPosition1.y * edgePosition2.x);
		tangent1.y = floater * (deltaUVPosition2.y * edgePosition1.y - deltaUVPosition1.y * edgePosition2.y);
		tangent1.z = floater * (deltaUVPosition2.y * edgePosition1.z - deltaUVPosition1.y * edgePosition2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = floater * (-deltaUVPosition2.x * edgePosition1.x + deltaUVPosition1.x * edgePosition2.x);
		bitangent1.y = floater * (-deltaUVPosition2.x * edgePosition1.y + deltaUVPosition1.x * edgePosition2.y);
		bitangent1.z = floater * (-deltaUVPosition2.x * edgePosition1.z + deltaUVPosition1.x * edgePosition2.z);
		bitangent1 = glm::normalize(bitangent1);

		// triangle 2
		edgePosition1 = position3 - position1;
		edgePosition2 = position4 - position1;
		deltaUVPosition1 = uvPosition3 - uvPosition1;
		deltaUVPosition2 = uvPosition4 - uvPosition1;

		floater = 1.0f / (deltaUVPosition1.x * deltaUVPosition2.y - deltaUVPosition2.x * deltaUVPosition1.y);

		tangent2.x = floater * (deltaUVPosition2.y * edgePosition1.x - deltaUVPosition1.y * edgePosition2.x);
		tangent2.y = floater * (deltaUVPosition2.y * edgePosition1.y - deltaUVPosition1.y * edgePosition2.y);
		tangent2.z = floater * (deltaUVPosition2.y * edgePosition1.z - deltaUVPosition1.y * edgePosition2.z);
		tangent2 = glm::normalize(tangent2);

		bitangent2.x = floater * (-deltaUVPosition2.x * edgePosition1.x + deltaUVPosition1.x * edgePosition2.x);
		bitangent2.y = floater * (-deltaUVPosition2.x * edgePosition1.y + deltaUVPosition1.x * edgePosition2.y);
		bitangent2.z = floater * (-deltaUVPosition2.x * edgePosition1.z + deltaUVPosition1.x * edgePosition2.z);
		bitangent2 = glm::normalize(bitangent2);

		GLfloat advancedQuadVertices[] = 
		{
			// Positions            // normal         // TexCoords  // Tangent                          // Bitangent
			position1.x, position1.y, position1.z, normal.x, normal.y, normal.z, uvPosition1.x, uvPosition1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			position2.x, position2.y, position2.z, normal.x, normal.y, normal.z, uvPosition2.x, uvPosition2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			position3.x, position3.y, position3.z, normal.x, normal.y, normal.z, uvPosition3.x, uvPosition3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			position1.x, position1.y, position1.z, normal.x, normal.y, normal.z, uvPosition1.x, uvPosition1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			position3.x, position3.y, position3.z, normal.x, normal.y, normal.z, uvPosition3.x, uvPosition3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			position4.x, position4.y, position4.z, normal.x, normal.y, normal.z, uvPosition4.x, uvPosition4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// Setup plane VAO
		glGenVertexArrays(1, &advancedQuadVAO);
		glGenBuffers(1, &aQuadVBO);
		glBindVertexArray(advancedQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, aQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(advancedQuadVertices), &advancedQuadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
	}
	glBindVertexArray(advancedQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// Initialise an plane quad with verticies for the position, texture coordinates (Lazy initialisation is used here).
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
void ManualQuad::InitPlane() 
{
	GLfloat planeVertices[] = 
	{
		// Positions            
		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

// Initialise an simple quad with verticies for the position, texture coordinates (Lazy initialisation is used here).
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
void ManualQuad::InitSimpleQuad() 
{
	if (simpleQuadVAO == 0) 
	{
		GLfloat simpleQuadVertices[] = 
		{
			// Positions   // TexCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};
		glGenVertexArrays(1, &simpleQuadVAO);
		glGenBuffers(1, &simpleQuadVBO);
		glBindVertexArray(simpleQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, simpleQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(simpleQuadVertices), &simpleQuadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	}
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// Initialise an alternative quad with verticies for the position, texture coordinates (Lazy initialisation is used here). (only 4 points as opposed to 6 as some verts share position)
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
void ManualQuad::RenderAltQuad()
{
	if (altQuadVAO == 0)
	{
		GLfloat altQuadVertices[] = 
		{
			// Positions        // Texture Coords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
			1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		};
		glGenVertexArrays(1, &altQuadVAO);
		glGenBuffers(1, &altQuadVBO);
		glBindVertexArray(altQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, altQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(altQuadVertices), &altQuadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(altQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

// Initialise a shadow plane quad with verticies for the position, normal and texture coordinates (Lazy initialisation is used here).
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
void ManualQuad::InitShadowPlane() 
{
	GLfloat splaneVertices[] = 
	{
		// Positions            // Normals           // Texture Coords
		25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  25.0f, 0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  25.0f,
		-25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

		25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  25.0f, 0.0f,
		25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  25.0f, 25.0f,
		-25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  25.0f
	};
	glGenVertexArrays(1, &splaneVAO);
	glGenBuffers(1, &splaneVBO);
	glBindVertexArray(splaneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, splaneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(splaneVertices), &splaneVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

// Initialise an blinn plane quad with verticies for the position, normal and texture coordinates (Lazy initialisation is used here).
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
void ManualQuad::InitBlinnPlane() 
{
	GLfloat blinnPlaneVertices[] = 
	{
		// Positions          // Normals         // Texture Coords
		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
		-8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f
	};

	glGenVertexArrays(1, &blinnPlaneVAO);
	glGenBuffers(1, &blinnPlaneVBO);
	glBindVertexArray(blinnPlaneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, blinnPlaneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blinnPlaneVertices), &blinnPlaneVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

// Initialise a transparent/window quad with verticies for the position, texture coordinates (Lazy initialisation is used here).
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
void ManualQuad::InitTransparentPlane() 
{
	if (transparentVAO == 0) 
	{
		GLfloat transparentVertices[] = 
		{
			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};
		glGenVertexArrays(1, &transparentVAO);
		glGenBuffers(1, &transparentVBO);
		glBindVertexArray(transparentVAO);
		glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(0);
}

// Initialise a screen quad with verticies for the position, texture coordinates (Lazy initialisation is used here).
// The relevant Vertex Array Object is generated, similar to the Vertex Buffer Object - bind and assign attributes as appropriate. Finally freeing/unbinding the vertex array.
void ManualQuad::InitScreen() {
	if (screenQuadVAO == 0) {
		GLfloat quadVertices[] = {
			// Positions   // TexCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};
		glGenVertexArrays(1, &screenQuadVAO);
		glGenBuffers(1, &screenQuadVBO);
		glBindVertexArray(screenQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	}
	glBindVertexArray(0);
}