#ifndef ManualCube_HEADER
#define ManualCube_HEADER

#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~17

class ManualCube 
{
	// Class for using a manually created cube primitive.
public:
	// Externally Accessible.
	ManualCube::ManualCube();
	void ManualCube::InitSimpleCube();
	void ManualCube::RenderAltCube();
	void ManualCube::InitContainer();
	void ManualCube::RenderSkybox();
	
	GLuint	containerVertexArrayObject = 0,
			lightVertexArrayObject = 0,
			cubeVAO = 0;

private:
	// Internally Accessible.
	GLuint	skyboxVertexArrayObject = 0, skyboxVertexBufferObject,
			altCubeVAO = 0, altCubeVBO, cubeVBO,
			containerVertexBufferObject;
	
	GLfloat scaleAmount, rotateAmount, angle, angleOverTime;

	glm::mat4 model;
	Camera cam;
};
#endif ManualCube_HEADER
