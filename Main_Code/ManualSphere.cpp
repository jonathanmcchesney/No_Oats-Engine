#include "ManualSphere.h"
#include "ShaderConstants.h"
#include "AssetManager.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~78

// Constructor for the Manually created Sphere class.
ManualSphere::ManualSphere() 
{
}

// This method allows us to pogramatically render a sphere (uses lazy initialisation). - Inspiration taken from opengl.com & Epic Games research paper on BRDF.
// We set up our VAO, VBO & VEO as well as the vecotrs for poisitions, UV (tex coords), normals & indicies.
// We then decide on how many segements we want for the x and t portions of the spehere (a sphere is unique because it holds more data horizontaly than the verticle aspects do.
// We then iterate through all rings and segments creating the xyz positions using the relevant cosine or sine funcs to push our positions, uv tex coordinates and normals back.
// We then iterate through sequential odd/even rows pushing the indicies back (populating it along with the data).
// We then bind our VAO's as normal etc, drawing the elements and unbinding the vertex array.
void ManualSphere::renderSphere()
{
	if (sphereVertexArrayObject == 0)
	{
		glGenVertexArrays(1, &sphereVertexArrayObject);

		unsigned int sphereVertexBufferObject, sphereElementBufferObject;
		glGenBuffers(1, &sphereVertexBufferObject);
		glGenBuffers(1, &sphereElementBufferObject);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;	// used to calculate t & b
		std::vector<glm::vec3> n;	// normals
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64, Y_SEGMENTS = 64; // increase to make sphere have more rings and segments
		const float PI = 3.14159265359;

		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS,
					ySegment = (float)y / (float)Y_SEGMENTS,
					xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI),
					yPos = std::cos(ySegment * PI),
					zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				n.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow)
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();
		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (n.size() > 0)
			{
				data.push_back(n[i].x);
				data.push_back(n[i].y);
				data.push_back(n[i].z);
			}
		}
		glBindVertexArray(sphereVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphereVertexArrayObject);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}