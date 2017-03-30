
#include "Sphere.h"

//-------------------------------------------------------------------------------
// �ÿ�
//-------------------------------------------------------------------------------
void CSphere::Empty()
{
	radius = 0.0f;
}

//-------------------------------------------------------------------------------
// �Ƿ�Ϊ��
//-------------------------------------------------------------------------------
bool CSphere::IsEmpty() const
{
	return (radius == 0.0f);
}

//-------------------------------------------------------------------------------
// �Ƿ����ĳ����
//-------------------------------------------------------------------------------
bool CSphere::Contains(const Vector3& point) const
{
	float distSq = DistanceSquared(center, point);

	return (distSq <= (radius * radius));
}

//-------------------------------------------------------------------------------
// �Ƿ����ĳ����
//-------------------------------------------------------------------------------
bool CSphere::Contains(const CSphere& sphere) const 
{
	if (radius < sphere.radius)
		return false;

	float distSq = DistanceSquared(sphere.center, center);
	float radDif = radius - sphere.radius;
	return (distSq <= radDif * radDif);
}

//-------------------------------------------------------------------------------
// �������Ƿ��ཻ
//-------------------------------------------------------------------------------
bool IntersectSpheres(const CSphere &sphere1, const CSphere &sphere2)
{
	float distSq = DistanceSquared(sphere1.center, sphere2.center);
	float radSum = sphere1.radius + sphere2.radius;
	return (distSq <= radSum * radSum);
}
