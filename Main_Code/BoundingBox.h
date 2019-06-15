/*
An Axis Aligned Bounding Box
*/
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "Vector3.h"
#include "NoOatsBehaviour.h"
#include "Mesh.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include <math.h>
#include "Logging.h"
#include "GameObject.h"

class AxisAlignedBoundingBox : public NoOatsBehaviour
{
public:

	//Constructor
	AxisAlignedBoundingBox(GameObject* gameObject);

	//Mesh for the AABB to conform to
	Mesh mesh;

	//Variables
	float  halfWidth, halfHeight, halfDepth;
	OatsPhysics::Vector3 center;

	//Methods
	void Update();
	void DebugInfo();

private:
	float minX, maxX, minY, maxY, minZ, maxZ;
};
#endif // !BOUNDINGBOX_H