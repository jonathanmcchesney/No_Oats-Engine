#include "Shadows.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~130

// Shadow Constructor
Shadows::Shadows() 
{
}

// Rendering the directional shadow scene - we do this by setting our projection matrix (perspective as opposed to orthagonal). By setting the shader to be used we can then set the uniforms to
// be used within the shaders (projection and view). We then draw the directional primitives (cubes), if the debug quad is enabled then we switch shaders, set the uniforms and bind the depth map texture
// and finally render the quad to the screen.
void Shadows::RenderDirectionalShadowScene(AssetShader shadowShader, AssetShader debugQuad, Camera cam, bool shadowsEnabled, bool quadDebugEnabled, glm::mat4 &projection)
{
	projection = glm::perspective(cam.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	shadowShader.use();
	
	shadowShader.setMat4("projection", projection);
	shadowShader.setMat4("view", view);

	DrawDirectionalPrimitives(shadowShader, 4, cam);

	if (quadDebugEnabled == false) 
	{
		debugQuad.use();
		debugQuad.setFloat("near_plane", near_plane);
		debugQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		Shadows::quad.RenderAltQuad();
	}
}

// This allows us to draw the directional primitives (cubes that cast shadows on the floor). We begin by setting the uniforms in the shaders, as well as binfing the appropriate texture
// and depth map. We then iterate through all the cubes we want to draw, updating them, setting their model uniform and then rendering the cube. Finally repeating this process for the
// programatically created sphere.
void Shadows::DrawDirectionalPrimitives(AssetShader shader, int totalItems, Camera cam) 
{
	glUniform3fv(glGetUniformLocation(shader.program, "lightPos"), 1, &slightPos[0]);
	glUniform3fv(glGetUniformLocation(shader.program, "viewPos"), 1, &cam.position[0]);
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	shader.setInt("shadows", shadowsEnabled);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("metalTexture"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	for (int i = 0; i < totalItems; i++) 
	{
		UpdateDirectionalPrimitives(i);
		shader.setMat4("model", model);
		cube.RenderAltCube();
	}
	shader.setMat4("model", model);
	Shadows::sphere.renderSphere();
}

// This allows us to update the location, scale and rotation of our primititves.
void Shadows::UpdateDirectionalPrimitives(int i) 
{
	model = glm::mat4();
	if (i == 0) 
	{
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, -0.3));
	}
	else if (i == 1) 
	{
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.5));
	}
	else if (i == 2) 
	{
		model = glm::translate(model, glm::vec3(-1.4f, 0.0f, 1.4));
		model = glm::rotate(model, 65.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.6));
	}
	else if (i == 3) 
	{
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -1.0));
		model = glm::rotate(model, 65.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.4));
	}
}

// Renders the point shadow / omni-directional shadow scene. We do this by setting our projection (perspective) similar to the directional shadow scene.
// We then set the appropriate uniforms in the shaders. Finally calling the draw point primitives method.
void Shadows::RenderPointShadowScene(AssetShader pShadowShader, Camera cam, bool activeRoom, glm::mat4 &projection)
{
	projection = glm::perspective(cam.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	pShadowShader.use();
	pShadowShader.setMat4("projection", projection);
	pShadowShader.setMat4("view", view);

	DrawPointPrimitives(pShadowShader, 5, activeRoom, cam);
}

// This allows us to draw the point primitives (cubes that cast shadows on the cubemap faces). We begin by setting the uniforms in the shaders, as well as binfing the appropriate texture
// and depth map. We then iterate through all the cubes we want to draw, updating them, setting their model uniform and then rendering the cube. This process is unique because we decide
// whether the surrounding cube map will be displayed, if so we then update the shader uniforms as appropriate - the normals are important as inversing their direction results in the normals
// now facing roughly in the +z direction.
void Shadows::DrawPointPrimitives(AssetShader shader, int totalItems, bool activeRoom, Camera cam) 
{
	glUniform3fv(glGetUniformLocation(shader.program, "lightPos"), 1, &pslightPos[0]);
	glUniform3fv(glGetUniformLocation(shader.program, "viewPos"), 1, &cam.position[0]);
	shader.setInt("shadows", shadowsEnabled);
	shader.setFloat("far_plane", far);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("metalTexture"));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

	if (activeRoom) 
	{
		UpdatePointPrimitives(0);
		shader.setMat4("model", model);
		glCullFace(GL_FRONT);
		shader.setInt("reverse_normals", 1);
		Shadows::cube.RenderAltCube();
		shader.setInt("reverse_normals", 0);
		glCullFace(GL_BACK);
	}
	model = glm::mat4();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("altTexture"));
	for (int i = 1; i < 5; i++) 
	{
		UpdatePointPrimitives(i);
		shader.setMat4("model", model);
		Shadows::cube.RenderAltCube();
	}
}

// This allows us to update the location, scale and rotation of our primititves.
void Shadows::UpdatePointPrimitives(int i) 
{
	model = glm::mat4();
	if (i == 0)
	{
		model = glm::scale(model, glm::vec3(16.0));
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0));
	}
	else if (i == 1)
	{
		model = glm::translate(model, glm::vec3(4.0f, 3.5f, 0.0));
	}
	else if (i == 2)
	{
		model = glm::translate(model, glm::vec3(2.0f, 3.0f, 3.0));
		model = glm::scale(model, glm::vec3(1.5));
	}
	else if (i == 3)
	{
		model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	}
	else if (i == 4) 
	{
		model = glm::translate(model, glm::vec3(-1.5f, 2.5f, -3.0));
		model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.5));
	}
}

// This method is where we generate our two shadow depth maps, 1) directional:- 2Dtexture 2) point:- cubemap.
// We create our light projection matrix (its orthagonal this time), light view (using the glm::lookat func to show from the lights POv, the light space matrix which
// is a combination of the light position and view i.e. from the lights pov - allows us to render the scene from the lights point of view.
// The depth shader is used, the uniforms are set, glViewport is updated to take a region that fits the shadow resolution correctly. The depth map framebuffer object is bound,
// making sure to clear the depth buffer before binding the relevant vertex array opject and drawing our plane and primitives - finally unbinding the framebuffer.
void Shadows::RenderDepthMap(AssetShader shadowDepthShader, AssetShader pShadowDepthShader, bool activeRoom, Camera cam, glm::mat4 &projection)
{
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(slightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	shadowDepthShader.use();
	shadowDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowDepthShader.setMat4("model", model);
	glBindVertexArray(Shadows::quad.blinnPlaneVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	DrawDirectionalPrimitives(shadowDepthShader, 4, cam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// These shadow transformations relate to looking at each of the cubemaps faces, 90.0f is used in the shadow projections so we can fit when rendering
	shadowProjections = glm::perspective(90.0f, aspect, near, far);
	shadowTransformations.push_back(shadowProjections * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransformations.push_back(shadowProjections * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransformations.push_back(shadowProjections * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransformations.push_back(shadowProjections * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransformations.push_back(shadowProjections * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransformations.push_back(shadowProjections * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	// The glViewport is updated as appropriately for the point shadow depth map & Framebuffer object. 
	glViewport(0, 0, POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, pointDepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	pShadowDepthShader.use();

	// We iterate through all faces setting the relevant attributes within the shader to be used.
	for (GLuint i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(glGetUniformLocation(pShadowDepthShader.program, ("shadowTransforms[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransformations[i]));
	}
	
	// Finally we set our uniforms and draw the point primitives before unbinding the framebuffer object.
	pShadowDepthShader.setInt("far_plane", far);
	glUniform3fv(glGetUniformLocation(pShadowDepthShader.program, "lightPos"), 1, &pslightPos[0]);
	DrawPointPrimitives(pShadowDepthShader, 5, activeRoom, cam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}