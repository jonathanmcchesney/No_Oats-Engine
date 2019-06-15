#include "BoundingBox.h"

class AABBCollisionDetection
{
public:
	//Simple AABB to AABB check
	static bool TestAABBToAABB(GameObject* gameObject1, GameObject* gameObject2);
};