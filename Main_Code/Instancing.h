#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~15

class Instancing
{
	// This is the instancing class which holds methods for rendering our instancing scene. 
	// The instance total is set in here as a placeholder & can be altered.
public:
	// Accessible Externally
	Instancing::Instancing();
	void Instancing::DrawInstancingScene(AssetShader simpleInstanceShader, AssetShader instanceShader, Model bigModel, Model instanceModel, Camera cam);
	void Instancing::InitInstancingScene(Model model, GLuint buffer);
	glm::mat4* Instancing::InitInstancingModels();
	glm::mat4* modelMatrices;

private:
	// Accessible Internally
	void Instancing::Draw(AssetShader simpleInstanceShader, AssetShader instanceShader, Model bigModel, Model instanceModel, Camera cam);
	void Instancing::Update(int totalItems);
	void Instancing::UpdateInstancingModels(GLuint i);

	GLuint instanceTotal = 20000, instancingVertexArrayObject;
	GLfloat radius = 50.0f, offset = 26.0f, deltaAngle, displacement, xValue, yValue, zValue, scaleValue, rotationAngle;
	glm::mat4 model;
	Camera cam;
};



