#ifndef SCALESCRIPT_H
#define SCALESCRIPT_H
#include "Transform.h";
#include "Vector3.h";
#include "NoOatsBehaviour.h"
#include "GameObject.h"

class ScaleScript : public NoOatsBehaviour
{
public:

	//Constrcutor
	ScaleScript(OatsPhysics::Vector3 scale)
	{
		myScale = scale;
		name = "ScaleScript";
	};

	//Called upon Script Start
	virtual void Start()
	{
		myTransform = &gameObject->transform;
	};

	//Called once every main update loop
	virtual void Update()
	{
		myTransform->Scale.x += myScale.x;
		myTransform->Scale.y += myScale.y;
		myTransform->Scale.z += myScale.z;
	};

	//Provides debug information
	virtual void DebugInfo()
	{
		Logging::logMessageOnly("Attached to : " + gameObject->tag);
		Logging::logMessageOnly("Scale vector : " + myScale.ToString());
	}
private:

	//The Transform this Script affects
	Transform* myTransform;

	//The Vector3 to scale by
	OatsPhysics::Vector3 myScale;
};
#endif // !SCALESCRIPT_H