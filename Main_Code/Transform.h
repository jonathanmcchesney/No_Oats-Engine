#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector3.h"

//This class provides a way to position, scale and rotate all GameObjects

	class Transform {
	public:
		OatsPhysics::Vector3 Translation;
		OatsPhysics::Vector3 Scale;
		OatsPhysics::Vector3 Rotation;

	public:
		Transform() {
			OatsPhysics::Vector3 Translation = OatsPhysics::Vector3();
			OatsPhysics::Vector3 Scale = OatsPhysics::Vector3();
			OatsPhysics::Vector3 Rotation = OatsPhysics::Vector3();
		}
	};

#endif
