#ifndef ManualSphere_HEADER
#define ManualSphere_HEADER

#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~9

class ManualSphere
{
public:
	// Externally Accessible
	ManualSphere::ManualSphere();
	void ManualSphere::renderSphere();

private:
	// Internally Accessible
	GLuint sphereVertexArrayObject = 0, indexCount;

	glm::mat4 model;
	Camera cam;
};
#endif ManualSphere_HEADER
