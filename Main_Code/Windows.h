#include "Camera.h"
#include "Constants.h"
#include "Shader.h"
#include "ManualQuad.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~14

class Windows
{
	// This class holds functionality showing off our development of "blending" functionality
	// and relevant user stories. Public and private accesses are assigned to limit visible methods and variables.
public:
	// Accessible Externally
	Windows::Windows();
	void Windows::Draw(AssetShader &windowShader, Camera cam, glm::mat4 &projection);  // Ran profiller CodeXL - highlighted a bottleneck here - solved 
	void Windows::SortWindows(Camera cam);
	void Windows::SetupWindows();
	ManualQuad Windows::quad;

private:
	// Accessible Internally
	void Windows::Update(GLuint i);
	std::vector<glm::vec3> translucentWindows;
	std::map<GLfloat, glm::vec3> sorted;
	bool isWindowsSetup = false;

	glm::mat4 model;
	Camera cam;
};