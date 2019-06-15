#include "CollisionDetection.h"
#include "Logging.h"


//Simple AABB to AABB Collision Detection
bool AABBCollisionDetection::TestAABBToAABB(GameObject* gameObject1, GameObject* gameObject2)
{
	bool result = false;

	//AABBs
	AxisAlignedBoundingBox* box1 = gameObject1->GetScript<AxisAlignedBoundingBox>();
	AxisAlignedBoundingBox* box2 = gameObject2->GetScript<AxisAlignedBoundingBox>();

	//Check for Overlap
	if (box1->center.x + box1->halfWidth > box2->center.x - box2->halfWidth &&
		box1->center.x - box1->halfWidth < box2->center.x + box2->halfWidth &&
		box1->center.y + box1->halfHeight > box2->center.y - box2->halfHeight &&
		box1->center.y - box1->halfHeight < box2->center.y + box2->halfHeight &&
		box1->center.z + box1->halfDepth > box2->center.z - box2->halfDepth &&
		box1->center.z - box1->halfDepth < box2->center.z + box2->halfDepth)
	{
		Logging::logMessageOnly("Collision Detected");
		result = true;
	}

	return result;
}