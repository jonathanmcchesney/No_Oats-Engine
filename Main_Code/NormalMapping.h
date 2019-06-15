#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ManualQuad.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~12

class NormalMapping
{
	// Class for using normal mapping functionality.
public:
	// Accessible Externally
	NormalMapping::NormalMapping();
	void NormalMapping::RenderNormalMappingScene(AssetShader &tangentNormalShader, Camera cam, glm::mat4 &projection);

private:
	// Accessible Internally
	void NormalMapping::Draw(AssetShader &tangentNormalShader, Camera cam);
	void NormalMapping::Update(int rotation);

	ManualQuad NormalMapping::quad;
	glm::vec3 wall;
	glm::mat4 model;
	Camera cam;
};