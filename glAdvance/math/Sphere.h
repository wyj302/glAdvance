//////////////////////////////////////////////////////////////////////////
//  CSphere类，球类，方便渲染和碰撞检测
//
//  Sphere.h: Sphere类的声明
//
//  Copy Rights Wonderful 2006
//////////////////////////////////////////////////////////////////////////

#ifndef __SPHERE_H_INCLUDED__
#define __SPHERE_H_INCLUDED__

#include "Vector3.h"

//-------------------------------------------------------------------------------
// 球类
//-------------------------------------------------------------------------------
class CSphere
{ 
public:
	// 构造函数
	CSphere() { }
	CSphere(const Vector3& pos, float rad) : center(pos), radius(rad) { if (radius < 0.0f) radius = 0.0f; }

	// 置空
	void Empty();
	// 是否为空
	bool IsEmpty() const;

	// 是否包含某个点
	bool Contains(const Vector3& point) const;
	// 是否包含某个球
	bool Contains(const CSphere& sphere) const;

public:
	Vector3  center;		// 中心点
	float    radius;		// 半径
};

// 两个球是否相交
bool IntersectSpheres(const CSphere &sphere1, const CSphere &sphere2);


#endif 