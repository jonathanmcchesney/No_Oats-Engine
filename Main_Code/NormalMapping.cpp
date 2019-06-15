#include "NormalMapping.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~40

// Normal Mapping Constructor
NormalMapping::NormalMapping()
{
	glm::mat4 projection = projection;
}

// This allows us render our quad primitives to screen by assigning a diffuse and normal map.
// This method begins by setting our model matrix and passing it through to the vertex shader, the uniforms are then set for the light (sLightPos) and view (cam.position) position.
// We then make active and bind our diffuse and normal map before rendering our advanced quad to screen. 
void NormalMapping::Draw(AssetShader &tangentNormalShader, Camera cam) 
{
	tangentNormalShader.setMat4("model", model);
	glUniform3fv(glGetUniformLocation(tangentNormalShader.program, "lightPos"), 1, &slightPos[0]);
	glUniform3fv(glGetUniformLocation(tangentNormalShader.program, "viewPos"), 1, &cam.position[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("nDiffuseMap"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("nNormalMap"));
	NormalMapping::quad.RenderAdvancedQuad();
}

// We are able to update the position and orientation of the normal mapped brick wallsby passing in an int to determine (back, left or right) wall locations.
void NormalMapping::Update(int rotation) 
{
	model = glm::mat4();
	model = glm::translate(model, wall);
	if (rotation == 0)
	model = glm::rotate(model, 0.0f, glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
	if (rotation == 1)
	model = glm::rotate(model, 3.14f / 2, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	if (rotation == 2)
	model = glm::rotate(model, -3.14f / 2, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
}

// This allows us to render the normal mapped wall scene. We begin by using our normal shader, set the view matrix as well as the perspective projection matrix - using these to set the uniforms in the shader.
// We then iterate through the back, left and right walls (intgers) as we update and draw the quads in the relevant location.
void NormalMapping::RenderNormalMappingScene(AssetShader &tangentNormalShader, Camera cam, glm::mat4 &projection)
{
	tangentNormalShader.use();
	view = cam.GetViewMatrix();
	projection = glm::perspective(cam.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	tangentNormalShader.setMat4("view", view);
	tangentNormalShader.setMat4("projection", projection);

	for (int backWall = -6; backWall < 8; backWall += 2) 
	{
		wall = glm::vec3((GLfloat)backWall, 0.49f, -3.0f);
		Update(0);
		Draw(tangentNormalShader, cam);
	}

	for (int leftWall = -2; leftWall < 8; leftWall += 2) 
	{
		wall = glm::vec3(-7.0f, 0.49f, (GLfloat)leftWall);
		Update(1);
		Draw(tangentNormalShader, cam);
	}

	for (int rightWall = -2; rightWall < 8; rightWall += 2) 
	{
		wall = glm::vec3(7.0f, 0.49f, (GLfloat)rightWall);
		Update(2);
		Draw(tangentNormalShader, cam);
	}
}