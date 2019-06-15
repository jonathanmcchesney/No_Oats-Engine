#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "Shader.h"
#include "ManualCube.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~9

class Skybox
{
	// Class for using the skybox
public:
	// Accessible Externally
	Skybox::Skybox();
	void Skybox::RenderSkyboxScene(AssetShader shader, Camera cam, glm::mat4 &projection);
	GLuint Skybox::RandomSkybox();

private:
	// Accessible Internally
	ManualCube Skybox::skybox;

	glm::mat4 model;
	Camera cam;
};