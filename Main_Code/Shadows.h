#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ManualCube.h"
#include "ManualQuad.h"
#include "ManualSphere.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~16

class Shadows
{
	// Class for using shadow functionality.
public:
	// Accessible Externally
	Shadows::Shadows();
	void Shadows::RenderDirectionalShadowScene(AssetShader shadowShader, AssetShader debugQuad, Camera cam, bool shadowsEnabled, bool quadDebugEnabled, glm::mat4 &projection);
	void Shadows::RenderPointShadowScene(AssetShader pShadowShader, Camera cam, bool activeRoom, glm::mat4 &projection);
	void Shadows::RenderDepthMap(AssetShader shadowDepthShader, AssetShader pShadowDepthShader, bool activeRoom, Camera cam, glm::mat4 &projection);

	ManualQuad Shadows::quad;

private:
	// Accessible Internally
	void Shadows::DrawPointPrimitives(AssetShader shader, int totalItems, bool activeRoom, Camera cam);
	void Shadows::UpdatePointPrimitives(int i);
	void Shadows::DrawDirectionalPrimitives(AssetShader shader, int totalItems, Camera cam);
	void Shadows::UpdateDirectionalPrimitives(int i);
	ManualCube Shadows::cube;
	ManualSphere Shadows::sphere;

	glm::mat4 model;
};
