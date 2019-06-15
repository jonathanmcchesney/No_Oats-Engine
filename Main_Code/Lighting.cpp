#include "Lighting.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~80

// Constructor for the lighting class
Lighting::Lighting() 
{
	glm::mat4 projection = projection;
}

// This method allows us to render the museum floor. We set our light and view position attributes (Specify the value of a uniform variable for the current program object).
// The Blinn shader blinn bool attribute is then set, if its true then we use the blinn-phong lighting model, if false then we use the phong lighting model on the ground. We then use the toggleShadow
// bool, if this is true then we use the shadow shader to draw shadows on the floor (with the blinn-phong lighting model applied. 
// We bind a quad vao, floor texture, draw the array of triangles and finally unbind the vertex array.
void Lighting::DrawFloor(AssetShader blinnShader, AssetShader shadowShader, Camera cam) 
{
	glUniform3fv(glGetUniformLocation(blinnShader.program, "lightPos"), 1, &slightPos[0]);
	glUniform3fv(glGetUniformLocation(blinnShader.program, "viewPos"), 1, &cam.position[0]);
	blinnShader.setInt("blinn", blinn);

	if (toggleShadowBlinn == true) 
	{
		shadowShader.use();
		shadowShader.setMat4("model", model);
	}

	glBindVertexArray(Lighting::quad.blinnPlaneVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("floorTexture"));

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// This method renders the blinn scene by initialising the blinn plane(floor quad), using the blinn lighting shader and setting the view and projection attributes for the shader.
// The draw floor method is then called, passing the blinn and shadow shader.
void Lighting::RenderBlinnScene(AssetShader blinnShader, AssetShader shadowShader, Camera cam, glm::mat4 &projection)
{
	Lighting::quad.InitBlinnPlane();
	blinnShader.use();

	blinnShader.setMat4("view", view);
	blinnShader.setMat4("projection", projection);

	DrawFloor(blinnShader, shadowShader, cam);
}

// Updates the lamp object position and scale.
void Lighting::UpdateLamp() 
{
	model = glm::mat4();
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, glm::vec3(0.03f));
}

// Updates the point light objects position and scale.
void Lighting::UpdatePointLights(GLuint currentLight) 
{
	model = glm::mat4();
	model = glm::translate(model, pointLightPositions[currentLight]);
	model = glm::scale(model, glm::vec3(0.02f));
}

// This allows us to draw the lights as objects (models), we first set the view, projection and model location, updating the lamps position and drawing the model, finally freeing the vertex array.
// We then bind our cube light vertex aray and iterate through all point lights updating their position and drawing their models, then we unbind the vertex aray.
void Lighting::DrawLights(AssetShader lampShader, AssetShader modelShader, glm::mat4 &projection)
{
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	UpdateLamp();
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	lightBulb.Draw(modelShader);
	glBindVertexArray(0);

	glBindVertexArray(Lighting::cube.lightVertexArrayObject);
	for (GLuint i = 0; i < 4; i++)
	{
		UpdatePointLights(i);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		lightBulb.Draw(modelShader);
	}
	glBindVertexArray(0);
}

// This renders the light scene by setting the uniforms and drawing the light objects.
void Lighting::RenderLightScene(AssetShader lampShader, AssetShader modelShader, glm::mat4 &projection)
{
	lampShader.use();
	modelLocation = glGetUniformLocation(lampShader.program, "model");
	viewLocation = glGetUniformLocation(lampShader.program, "view");
	projectionLocation = glGetUniformLocation(lampShader.program, "projection");

	DrawLights(lampShader, modelShader, projection);
}

// This allows us to set up our lighting parameters to be passed on to the shaders.
// This includes a specular/shiny component, emission aspects, direction/position, ambient, diffuse (constant, linear & quadratic) as well as cut off regions.
void Lighting::SetupLightParams(AssetShader lightingShader, Camera cam)
{
	lightingShader.use();
	viewPositionLocation = glGetUniformLocation(lightingShader.program, "viewPos");
	glUniform3f(viewPositionLocation, cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
	glUniform1f(glGetUniformLocation(lightingShader.program, "material.shininess"), 32.0f);
	glUniform1f(glGetUniformLocation(lightingShader.program, "material.emission"), 2);

	glUniform3f(glGetUniformLocation(lightingShader.program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(lightingShader.program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(lightingShader.program, "dirLight.diffuse"), 0.45f, 0.45f, 0.45f);
	glUniform3f(glGetUniformLocation(lightingShader.program, "dirLight.specular"), 0.69f, 0.69f, 0.69f);

	glUniform3f(glGetUniformLocation(lightingShader.program, "spotLight.position"), cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
	glUniform3f(glGetUniformLocation(lightingShader.program, "spotLight.direction"), cam.GetFront().x, cam.GetFront().y, cam.GetFront().z);
	glUniform3f(glGetUniformLocation(lightingShader.program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(lightingShader.program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(lightingShader.program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(lightingShader.program, "spotLight.emission"), 2);
	glUniform1f(glGetUniformLocation(lightingShader.program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightingShader.program, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(lightingShader.program, "spotLight.quadratic"), 0.064f);
	glUniform1f(glGetUniformLocation(lightingShader.program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(lightingShader.program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

	for (GLuint i = 0; i < 4; i++)
	{
		pointLightID = to_string(i);
		glUniform3f(glGetUniformLocation(lightingShader.program, ("pointLights[" + pointLightID + "].position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
		glUniform3f(glGetUniformLocation(lightingShader.program, ("pointLights[" + pointLightID + "].ambient").c_str()), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.program, ("pointLights[" + pointLightID + "].diffuse").c_str()), 0.85f, 0.6f, 0.75f);
		glUniform3f(glGetUniformLocation(lightingShader.program, ("pointLights[" + pointLightID + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.program, ("pointLights[" + pointLightID + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.program, ("pointLights[" + pointLightID + "].linear").c_str()), 0.095f);
		glUniform1f(glGetUniformLocation(lightingShader.program, ("pointLights[" + pointLightID + "].quadratic").c_str()), 0.064f);
	}
}

// This allows us to update the light positions (shadow, normal and point shadow light xyz components).
void Lighting::UpdateLightPositions()
{
	slightPos.x = sin(glfwGetTime()) * 3.0f;
	slightPos.z = cos(glfwGetTime()) * 2.0f;
	slightPos.y = 5 + cos(glfwGetTime()) * 1.0f;
	nLightPos.x = sin(glfwGetTime()) * 3.0f;
	nLightPos.z = cos(glfwGetTime()) * 2.0f;
	nLightPos.y = 0 + cos(glfwGetTime()) * 1.0f;
	pslightPos.x = sin(glfwGetTime()) * 3.0f;
	pslightPos.z = cos(glfwGetTime()) * 2.0f;
	pslightPos.y = 0 + cos(glfwGetTime()) * 1.0f;
}
