#include "Depth.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~60

// Depth Scene Constructor
Depth::Depth()
{
	glm::mat4 projection = projection;
}

// This draw method (which takes an asset shader as a parameter) allows us to render our cube objects to screen.
// We begin by updating the objects, with the first object updated being a quad with a unique wood texture,
// we then iterate over our 6 cube objects updating their positioning (&translations) as well as their attributes,
// textures and VAO's. Finally unbinding the vertex array. Culling is enabled here so that back faces are not drawn.
void Depth::Draw(AssetShader &depthShader)
{
	Update(0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("artTexture"));
	depthShader.setMat4("model", model);
	Depth::quad.RenderAltQuad();
	glEnable(GL_CULL_FACE); // Back faces are not drawn
	glBindVertexArray(0); // Unbind

	Depth::cube.InitSimpleCube();
	glBindVertexArray(Depth::cube.cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("cubeTexture"));

	for (int i = 1; i < 7; i++)
	{
		Update(i);
		depthShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0); // Unbind
}

// Update method for handling the positioning and translations of the depth infuenced objects.
void Depth::Update(int item) 
{
	if (item == 0)
	{
		glDisable(GL_CULL_FACE); // The quad shouldn't have back faces culled. 
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-6.0f, -0.05f, 1.67f));
		model = glm::scale(model, glm::vec3(0.5));
		model = glm::rotate(model, -35.0f, glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
	}
	else if (item == 1)
	{
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.9));
	} 
	else if (item == 2)
	{
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.9));
	}
	else if (item == 3)
	{
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-6.0f, 0.655f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4));
	}
	else if (item == 4)
	{
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.5f, -0.2f, 0.31f));
		model = glm::scale(model, glm::vec3(0.4));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	}
	else if (item == 5)
	{
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-6.3f, -0.03f, 0.11f));
		model = glm::rotate(model, 25.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.8));
	}
	else if (item == 6)
	{
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.7f, -0.38f, -0.05f));
		model = glm::scale(model, glm::vec3(0.25));
	}
}

// This method allows us to render the scene, taking a depth asset shader and camera object as params.
// We set our shader, along with the attributes to be sent to the shaders.
// Finally we call our draw method, passing the depth shader as a parameter.
void Depth::RenderDepthScene(AssetShader &depthShader, Camera cam, glm::mat4 &projection)
{
	depthShader.use();
	view = cam.GetViewMatrix();
	depthShader.setMat4("view", view);
	depthShader.setMat4("projection", projection);
	Draw(depthShader);
}
