#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ManualCube.h"
#include "ManualQuad.h"
#include "Lighting.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~14

class Container
{
	// This class contains our moving (translations) container scene which has multiple maps
	// applied and reacts to our phong lighting model (directional, point light &
	// flashlight. Public and private access is assigned to limit what is accessible outside of this class.
public:
	// Externally Accessible
	Container::Container();
	void Container::RenderMovingContainerScene(AssetShader lightingShader, Camera cam, glm::mat4 &projection);
	void Container::SetContainerImage(GLboolean default, int imageInt);

	Lighting Container::lights;

private:
	// Internally Accessible
	void Container::DrawContainer(AssetShader lightingShader, Camera cam, glm::mat4 &projection);
	void Container::UpdateContainer();

	ManualCube Container::cube;
	ManualQuad Container::quad;
	GLfloat scaleAmount, rotateAmount, angle, angleOverTime;
	Camera cam;
};