#ifndef VECTOR3_H
#define VECTOR3_H

#include <ostream>
#include <math.h>
#include <glm.hpp>
#include <string>
#include <iostream>
#include <sstream>

namespace OatsPhysics {
	class Vector3 {
	public:
		float x;
		float y;
		float z;


	public:

		//Constructors
		Vector3() : x(0), y(0), z(0) {};
		Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		//Construct a NoOatsPhyscis Vector3 from a GLM Vec3
		Vector3(glm::vec3 vec3)
		{
			this->x = vec3.x;
			this->y = vec3.y;
			this->z = vec3.z;
		}

		//Returns the Magnitude of the Vector
		float magnitude() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		//Returns the Magnitude of the Vector squared.
		float sqrMagnitude() const
		{
			return (x*x + y*y * z*z);
		}

		//Add Vectors
		void operator +=(Vector3 vector3ToAdd)
		{
			x += vector3ToAdd.x;
			y += vector3ToAdd.y;
			z += vector3ToAdd.z;
		}

		//Subtract Vectors
		void operator -=(Vector3 vector3ToSubtract)
		{
			x -= vector3ToSubtract.x;
			y -= vector3ToSubtract.y;
			z -= vector3ToSubtract.z;
		}

		//Add vectors and return the result
		Vector3 operator +(Vector3 vector3ToAdd)
		{
			return Vector3(x + vector3ToAdd.x, y + vector3ToAdd.y, z + vector3ToAdd.z);
		}

		//Subtract vectors and return the result
		Vector3 operator -(Vector3 vector3ToSubtract)
		{
			return Vector3(x - vector3ToSubtract.x, y - vector3ToSubtract.y, z - vector3ToSubtract.z);
		}

		//Add a scaled vector 
		void addScaledVector(Vector3 scaledVector, float scale)
		{
			x += scaledVector.x * scale;
			y += scaledVector.y * scale;
			z += scaledVector.z * scale;
		}

		//Multiply a vector
		void operator *= (float multiplier)
		{
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;
		}

		//Multiply and return a vector
		Vector3 operator * (float multiplier)
		{
			return Vector3(x * multiplier, y * multiplier, z * multiplier);
		}

		//Return the Scalar Product of a multiplication (x * x + y * y + z * z )
		float scalarProduct(Vector3 vectorToMultiply)
		{
			return (x * vectorToMultiply.x + y * vectorToMultiply.y + z * vectorToMultiply.z);
		}

		//Return the Vector Product of two vectors (y * z - Z * y 
		Vector3 vectorProduct(Vector3 vector3ToMultiply)
		{
			return Vector3(y*vector3ToMultiply.z - z*vector3ToMultiply.y,
				z*vector3ToMultiply.x - x*vector3ToMultiply.z,
				x*vector3ToMultiply.y - y*vector3ToMultiply.x);
		}

		//Converts the OatsPhysics - Vector3 to a GLM::Vec3
		glm::vec3 ToGLMVec3()
		{
			return glm::vec3(this->x, this->y, this->z);
		}

		//Returns the values of this Vector3 to a string
		std::string ToString()
		{
			std::ostringstream oss;
			oss << "X : " << this->x << " //  Y : " << this->y << " //  Z : " << this->z << std::endl;
			return oss.str();
		}
	};
}
#endif // !VECTOR3_H