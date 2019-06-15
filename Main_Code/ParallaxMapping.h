#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ManualQuad.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~14

class ParallaxMapping
{
	// Class for using parallax occlusion mapping functionality.
public:
	// Accessible Externally
	ParallaxMapping::ParallaxMapping();
	void ParallaxMapping::RenderParallaxMappingScene(AssetShader &parallaxShader, Camera cam, glm::mat4 &projection);

private:
	// Accessible Internally
	void ParallaxMapping::Draw(AssetShader &parallaxShader, Camera cam, int backWall);
	void ParallaxMapping::Draw(AssetShader &parallaxShader, Camera cam);
	void ParallaxMapping::Update(int backWall);
	void ParallaxMapping::Update();

	ManualQuad ParallaxMapping::quad;
	glm::vec3 wall;
	glm::mat4 model;
	Camera cam;
};