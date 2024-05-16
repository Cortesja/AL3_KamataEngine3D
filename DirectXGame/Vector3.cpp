#include "Vector3.h"
#include "Novice.h"
#include <math.h>

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{ 0 };
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result = { 0 };
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Multiply(float scalar, const Vector3& v)
{
	Vector3 result = { 0 };
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

float Dot(const Vector3& v1, const Vector3& v2)
{
	float result = ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
	return result;
}

float Length(const Vector3& v)
{
	Vector3 result;
	result.x = v.x * v.x;
	result.y = v.y * v.y;
	result.z = v.z * v.z;

	float length = sqrtf(result.x + result.y + result.z);
	return length;
}

Vector3 Normalize(const Vector3& v)
{
	Vector3 result;
	float len = Length(v);

	result.x = v.x / len;
	result.y = v.y / len;
	result.z = v.z / len;

	return result;
}