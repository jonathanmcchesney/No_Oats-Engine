#ifndef TRANSLATESCRIPT_H
#define TRANSLATESCRIPT_H
#include "Transform.h";
#include "Vector3.h";
#include "NoOatsBehaviour.h"
#include "GameObject.h"
#include "Logging.h"

class TranslateScript : public NoOatsBehaviour
{
public:

	//Constructor
	TranslateScript(OatsPhysics::Vector3 translation)
	{
		myTranslation = translation;
		name = "TranslateScript";
	};

	//Called upon Script Start
	virtual void Start()
	{
		myTransform = &gameObject->transform;
	};

	//Called once during every main update loop
	virtual void Update()
	{
		myTransform->Translation.x += myTranslation.x;
		myTransform->Translation.y += myTranslation.y;
		myTransform->Translation.z += myTranslation.z;
	};

	//Provides debug info
	virtual void DebugInfo()
	{
		Logging::logMessageOnly("Attached to : " + gameObject->tag);
		Logging::logMessageOnly("Translation vector :" + myTranslation.ToString());

	}

private:

	//The Transform this Script affects
	Transform* myTransform;

	//The Vector to Translate by
	OatsPhysics::Vector3 myTranslation;
};
#endif // !TRANSLATESCRIPT_H