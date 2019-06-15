#include "MeshRenderer.h"

//Default Constructor
MeshRenderer::MeshRenderer()
{
	this->model = Model("res/models/cube.obj");
	this->shader = AssetManager::getShader("cube");
	this->texture = AssetManager::getTexture("cubeTexture");
	this->name = "MeshRenderer";
}

//Configurable Constructor
MeshRenderer::MeshRenderer(Model model, AssetShader &shader)
{
	this->model = model;
	this->shader = shader;
	this->texture = AssetManager::getTexture("cubeTexture");
	this->name = "MeshRenderer";
}

//Update Method
void MeshRenderer::Update()
{
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader.program, "texture_diffuse"), 0);
	
	//Lighting Pointers
	GLint lightPosLoc = glGetUniformLocation(shader.program, "light.position");
	GLint viewPosLoc = glGetUniformLocation(shader.program, "viewPos");
	glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
	glUniform3f(viewPosLoc, gameObject->camera->GetPosition().x, gameObject->camera->GetPosition().y, gameObject->camera->GetPosition().z);
	
	//Lighting Properties
	glUniform3f(glGetUniformLocation(shader.program, "light.ambient"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(shader.program, "light.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(shader.program, "light.specular"), 5.4f, 5.4f, 5.4f);
	glUniform1f(glGetUniformLocation(shader.program, "material.shininess"), 32.0f);
	
	//Model View Projection Matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(gameObject->projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	
	//Model Transformations
	glm::mat4 meshModel;
	meshModel = glm::translate(meshModel, gameObject->transform.Translation.ToGLMVec3());
	meshModel = glm::scale(meshModel, gameObject->transform.Scale.ToGLMVec3());

	//Quaternion Based Rotation
	glm::quat quaternion;
	quaternion = glm::quat(gameObject->transform.Rotation.ToGLMVec3());

	//Apply the Rotation to the Rotation Matrix
	glm::mat4 RotationMatrix = glm::toMat4(quaternion);

	//Apply Rotation Matrix to the Model
	meshModel *= RotationMatrix;
	
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(meshModel));
	this->model.Draw(this->shader);
}