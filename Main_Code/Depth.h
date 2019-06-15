#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "Shader.h"
#include "ManualCube.h"
#include "ManualQuad.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~10

class Depth
{
	// Header class for depth functionality
public:
	// Externally Accessible
	Depth::Depth();
	void Depth::RenderDepthScene(AssetShader &depthShader, Camera cam, glm::mat4 &projection);

private:
	// Internally Accessible
	void Depth::Update(int item);
	void Depth::Draw(AssetShader &depthShader);

	ManualCube Depth::cube;
	ManualQuad Depth::quad;
	 
	Camera cam;
};