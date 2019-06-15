#include "Instancing.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~72

// Instancing Constructor
Instancing::Instancing() 
{
	glm::mat4 projection = projection;
}

// This method is used to draw the instancing objects, taking in the relevant Asset Shaders and Models (as well as a camera object) as params.
// We set our attributes to be sent to the shader for single object, we then switch to our instancing shader, unbinding the active texture and binding the models textures/meshes.
// We then iterate through all our models updating their positions (&translations), setting the attributes to be sent to the instance shader,
// binding the relevant vertex array object(s) and using the very important glDrawElementsInstanced method (draw multiple instances of a set of elements).
// Finally we unbind our vertex array and disable face culling.
void Instancing::Draw(AssetShader simpleInstanceShader, AssetShader instanceShader, Model bigModel, Model instanceModel, Camera cam) 
{
	simpleInstanceShader.setMat4("model", model);
	bigModel.Draw(simpleInstanceShader);

	instanceShader.use();
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, instanceModel.textures_loaded[0].id);
	// We iterate over all meshes for all models
	for (GLuint i = 0; i < instanceModel.meshes.size(); i++)
	{
		Update(1);
		AssetManager::getShader("instanceShader").setMat4("model", model);

		glBindVertexArray(instanceModel.meshes[i].VertexArrayObject);
		glDrawElementsInstanced(GL_TRIANGLES, instanceModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, instanceTotal);
		glBindVertexArray(0);
	}
	glDisable(GL_CULL_FACE);
}

// This method allows us to update our instanced models, with regards to their positioning and inducing a rotational effect on them.
void Instancing::Update(int totalItems) 
{
	model = glm::mat4();

	if (totalItems == 0) 
	{
		glEnable(GL_CULL_FACE);
		model = glm::translate(model, glm::vec3(2.0f, 9.5f, -1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	} 
	
	else if (totalItems == 1)
	model = glm::rotate(model, glm::radians((float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
}

// This allows us to draw our instanced scene, we use our shaders and set their view matrix as appropriate, update their positions (etc.) and then draw them.
void Instancing::DrawInstancingScene(AssetShader simpleInstanceShader, AssetShader instanceShader, Model bigModel, Model instanceModel, Camera cam) 
{
	simpleInstanceShader.use();
	simpleInstanceShader.setMat4("view", cam.GetViewMatrix());
	instanceShader.use();
	instanceShader.setMat4("view", view);
	simpleInstanceShader.use();

	Update(0);
	Draw(simpleInstanceShader, instanceShader, bigModel, instanceModel, cam);
}

// This initialises our instanced models, we create our model matricies matrix holding all our instanced model (setting its total to the instance total. A random seed ( CppReference.com - Seeds the pseudo-random number generator used by std::rand() with the value seed.)
// We then iterate over all our models and update their positioning (angle, displacement, offset & size - this creates a random and unique experience every time and creates an erratic 
// We return the modelMatricies matrix.
glm::mat4* Instancing::InitInstancingModels() 
{
	modelMatrices = new glm::mat4[instanceTotal];
	std::srand(glfwGetTime()); // random seed (so its different every time)

	for (GLuint i = 0; i < instanceTotal; i++)
		UpdateInstancingModels(i);
	
	return modelMatrices;
}

// This gives the models an unpredictable and erratic setup by using offsets, modulus and a random factor to displace, scale and rotate their individual positions.
// They are then put into orbit by slowly rotating about the center along the y axis (rotates horizontally)
void Instancing::UpdateInstancingModels(GLuint delta) 
{
	model = glm::mat4();

	deltaAngle = (GLfloat)delta / (GLfloat)instanceTotal * 360.0f;
	displacement = (rand() % (GLuint)(2.0 * offset * 100.0) ) / 100.0f - offset;
	
	xValue = sin(deltaAngle) * radius + displacement;
	displacement = (rand() % (GLuint)(2.0 * offset * 100.0) ) / 100.0f - offset;
	
	yValue = -2.65f + displacement * 0.405f;
	displacement = (rand() % (GLuint)(2.0 * offset * 100.0) ) / 100.0f - offset;
	
	zValue = cos(deltaAngle) * radius + displacement;
	model = glm::translate(model, glm::vec3(xValue, yValue, zValue));

	scaleValue = (rand() % 20) / 100.0f + 0.05;
	model = glm::scale(model, glm::vec3(scaleValue));

	rotationAngle = (rand() % 360);
	model = glm::rotate(model, rotationAngle, glm::vec3(0.4f, 0.6f, 0.8f));

	modelMatrices[delta] = model;
}

// This simply initiallises our scene by passing in our buffer object, generating ther relevant buffer and binding it. We then update the buffer data using instanceTotal * sizeof(glm::mat4).
// We then need to iterate over all the meshes and bind the vertex for every model... for every mesh. Finally unbinding when done with each vertex array.
void Instancing::InitInstancingScene(Model model, GLuint buffer) 
{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, instanceTotal * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (GLuint i = 0; i < model.meshes.size(); i++)
	{
		instancingVertexArrayObject = model.meshes[i].VertexArrayObject;
		glBindVertexArray(instancingVertexArrayObject);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}