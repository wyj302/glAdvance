
#include "Sphere.h"

//-------------------------------------------------------------------------------
// 置空
//-------------------------------------------------------------------------------
void CSphere::Empty()
{
	radius = 0.0f;
}

//-------------------------------------------------------------------------------
// 是否为空
//-------------------------------------------------------------------------------
bool CSphere::IsEmpty() const
{
	return (radius == 0.0f);
}

//-------------------------------------------------------------------------------
// 是否包含某个点
//-------------------------------------------------------------------------------
bool CSphere::Contains(const Vector3& point) const
{
	float distSq = DistanceSquared(center, point);

	return (distSq <= (radius * radius));
}

//-------------------------------------------------------------------------------
// 是否包含某个球
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
// 两个球是否相交
//-------------------------------------------------------------------------------
bool IntersectSpheres(const CSphere &sphere1, const CSphere &sphere2)
{
	float distSq = DistanceSquared(sphere1.center, sphere2.center);
	float radSum = sphere1.radius + sphere2.radius;
	return (distSq <= radSum * radSum);
}
