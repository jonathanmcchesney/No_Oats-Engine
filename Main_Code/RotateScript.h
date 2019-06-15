#ifndef ROTATESCRIPT_H
#define ROTATESCRIPT_H
#include "Transform.h";
#include "Vector3.h";
#include "NoOatsBehaviour.h"
#include "GameObject.h"

class RotateScript : public NoOatsBehaviour
{
public:

	//Constructor
	RotateScript(OatsPhysics::Vector3 rotation)
	{
		myRotation = rotation;
		name = "RotateScript";
	};

	//Called upon Script Start
	virtual void Start()
	{
		myTransform = &gameObject->transform;		
	};

	//Called once every main update loop
	virtual void Update()
	{
		myTransform->Rotation.x += myRotation.x;
		myTransform->Rotation.y += myRotation.y;
		myTransform->Rotation.z += myRotation.z;
	};

	//Provides debug information
	virtual void DebugInfo()
	{
		Logging::logMessageOnly("Attached to : " + gameObject->tag);
		Logging::logMessageOnly("Rotation vector : " + myRotation.ToString());
	}
private:
	//The Transform this Script affects
	Transform* myTransform;

	//The Rotation Vector 
	OatsPhysics::Vector3 myRotation;
};
#endif // !ROTATESCRIPT_H