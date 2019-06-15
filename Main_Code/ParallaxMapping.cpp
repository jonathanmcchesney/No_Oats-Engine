#include "ParallaxMapping.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~52

// Parallax Mapping Constructor
ParallaxMapping::ParallaxMapping()
{
	glm::mat4 projection = projection;
}

// This allows us to draw our parallax occlusion mapped images. We begin by setting our model matrix using the shaders wrapper method. We then set the uniforms for the light and view position (taking the slightpos
// and the cam.position respectively). We also use the wrapper methods to set the attributes for height scaling and whether parallax mapping is active. Then we make active and bind ou textures - diffuse, normal and height
// maps, finally rendering our advanced quad to screen. The quad draws and unbinds itself.
void ParallaxMapping::Draw(AssetShader &parallaxShader, Camera cam, int backWall) 
{
	parallaxShader.setMat4("model", model);
	glUniform3fv(glGetUniformLocation(parallaxShader.program, "lightPos"), 1, &slightPos[0]);
	glUniform3fv(glGetUniformLocation(parallaxShader.program, "viewPos"), 1, &cam.position[0]);
	parallaxShader.setFloat("height_scale", height_scale);
	parallaxShader.setInt("parallax", parallax_mapping);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture(diffuseMaps[(backWall + 6) / 2]));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture(normalMaps[(backWall + 6) / 2]));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture(heightMaps[(backWall + 6) / 2]));

	ParallaxMapping::quad.RenderAdvancedQuad();
}

// This draw method allows us to draw the alternative location parralax mapped image on the art stand, it function similar to the previous draw method
// except it disables and then reenables face culling at the end of the methood.
void ParallaxMapping::Draw(AssetShader &parallaxShader, Camera cam) 
{
	glDisable(GL_CULL_FACE);
	parallaxShader.setMat4("model", model);
	glUniform3fv(glGetUniformLocation(parallaxShader.program, "lightPos"), 1, &slightPos[0]);
	glUniform3fv(glGetUniformLocation(parallaxShader.program, "viewPos"), 1, &cam.position[0]);
	parallaxShader.setFloat("height_scale", height_scale);
	parallaxShader.setInt("parallax", parallax_mapping);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("metalTexture"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("pNormalMap10"));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("pHeightMap10"));
	
	ParallaxMapping::quad.RenderAdvancedQuad();
	glEnable(GL_CULL_FACE);
}

// This updates the position, scale and rotation of the art stand parralax image.
void ParallaxMapping::Update() 
{
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-4.85f, 0.49f, 1.0f));
	model = glm::scale(model, glm::vec3(0.3));
	model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	model = glm::rotate(model, 19.1f, glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
}

// This method allows us to update the location and scale of the various parralax images on the back wall.
void ParallaxMapping::Update(int backWall) 
{
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(backWall, 0.49f, -2.9f));
	model = glm::scale(model, glm::vec3(0.6));
}

// This allows us to draw the parallax mapped scene. We begin by using our parallax shader, setting the view and projection matricies and setting their values in the shaders.
// We then iterate through all the back wall parallax images updating and drawing as necessary. Finally updating and drawing the final quad image.
void ParallaxMapping::RenderParallaxMappingScene(AssetShader &parallaxShader, Camera cam, glm::mat4 &projection) 
{
	parallaxShader.use();
	view = cam.GetViewMatrix();
	projection = glm::perspective(cam.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	parallaxShader.setMat4("view", view);
	parallaxShader.setMat4("projection", projection);


	for (int backWall = -6; backWall < 8; backWall += 2) 
	{
		Update(backWall);
		Draw(parallaxShader, cam, backWall);
	}

	Update();
	Draw(parallaxShader, cam);
}
