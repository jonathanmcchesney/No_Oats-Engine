#include "BoundingBox.h"


AxisAlignedBoundingBox::AxisAlignedBoundingBox(GameObject* gameObject)
{
	mesh = gameObject->GetScript<MeshRenderer>()->model.meshes[0];


	if (mesh.vertices.size() <= 0)
	{
		Logging::logError("AxisAlighnedBoundingBos::Mesh Has No Vertices", __FILE__, __LINE__);
		return;
	}

	minX = mesh.vertices[0].Position.x;
	minY = mesh.vertices[0].Position.y;
	minZ = mesh.vertices[0].Position.z;
	
	//Anaylze and assigned Min/Max Vertices
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		//X Co-Ordinate
		if (mesh.vertices[i].Position.x < minX) minX = mesh.vertices[i].Position.x;
		if (mesh.vertices[i].Position.x > maxX) maxX = mesh.vertices[i].Position.x;

		//Y Co-Ordinate
		if (mesh.vertices[i].Position.y < minY) minY = mesh.vertices[i].Position.y;
		if (mesh.vertices[i].Position.y > maxY) maxY = mesh.vertices[i].Position.y;

		//Z Co-Ordinate
		if (mesh.vertices[i].Position.z < minZ) minZ = mesh.vertices[i].Position.z;
		if (mesh.vertices[i].Position.z > maxZ) maxZ = mesh.vertices[i].Position.z;
	}


	halfWidth = abs(minX);
	halfHeight = abs(minY);
	halfDepth = abs(minZ);

	//Assign Center-point
	center = OatsPhysics::Vector3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);

}

void AxisAlignedBoundingBox::Update()
{
	center = gameObject->transform.Translation;
}

void AxisAlignedBoundingBox::DebugInfo()
{
	std::cout << "Bounding Box Begin :: " << endl;
	std::cout << "Min X : " << minX << "     /     " << "Max X : " << maxX << endl;
	std::cout << "Min Y : " << minY << "     /     " << "Max Y : " << maxY << endl;
	std::cout << "Min Z : " << minZ << "     /     " << "Max Z : " << maxZ << endl;
	std::cout << "Center : " << center.x << " " << center.y << " " << center.z << endl;
	std::cout << " :: Bounding Box End " << endl;
}