#ifndef Lighting_HEADER
#define Lighting_HEADER

#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ManualCube.h"
#include "ManualQuad.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~16

class Lighting
{
	// This class demonstrates our lighting functionality and renders our lights as objects
public:
	// Externally Accessible
	Lighting::Lighting();
	void Lighting::RenderBlinnScene(AssetShader blinnShader, AssetShader shadowShader, Camera cam, glm::mat4 &projection);
	void Lighting::RenderLightScene(AssetShader lampShader, AssetShader modelShader, glm::mat4 &projection);
	void Lighting::SetupLightParams(AssetShader lightingShader, Camera cam);
	void Lighting::UpdateLightPositions();

	Model lightBulb;
	ManualQuad Lighting::quad;

private:
	// Internally Accessible
	void Lighting::DrawFloor(AssetShader blinnShader, AssetShader shadowShader, Camera cam);
	void Lighting::UpdateLamp();
	void Lighting::UpdatePointLights(GLuint currentLight);
	void Lighting::DrawLights(AssetShader lampShader, AssetShader modelShader, glm::mat4 &projection);

	ManualCube Lighting::cube;
	GLfloat scaleAmount, rotateAmount, angle, angleOverTime;
	Camera cam;
};
#endif