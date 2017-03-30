//////////////////////////////////////////////////////////////////////////
//  AABB3类，轴对齐盒子，方便渲染和碰撞检测
//
//  AABB3.h: AABB3类的声明
//
//////////////////////////////////////////////////////////////////////////

#ifndef __AABB3_H_INCLUDED__
#define __AABB3_H_INCLUDED__

#include "Vector3.h"

//-------------------------------------------------------------------------------
// 前向声明
//-------------------------------------------------------------------------------
class CMatrix44F;
class CSphere;
class CPlane;

//-------------------------------------------------------------------------------
// 轴对齐盒子
//-------------------------------------------------------------------------------
class AABB3
{
public:
	// 查询尺寸
	Vector3	Size() const { return max - min; }
	float	XSize() { return max.x - min.x; }
	float	YSize() { return max.y - min.y; }
	float	ZSize() { return max.z - min.z; }
	Vector3	Center() const { return (min + max) * .5f; }

	// 获取盒子的一个顶点
	Vector3	Corner(int i) const;


	// 盒子的一些通用操作

	// 把盒子置空
	void	Empty();

	// 增加一个点
	void	Add(const Vector3 &p);

	// 增加一个盒子
	void	Add(const AABB3 &box);

	// 变换盒子，并且计算新的盒子，新的盒子可能比原的大一些
	void	SetToTransformedBox(const AABB3 &box, const CMatrix44F &m);

	// 包含/重叠检测

	// 判断盒子是否为空。
	bool	IsEmpty() const;

	// 判断盒子是否包含特定点
	bool	Contains(const Vector3 &p) const;

	// 返回盒子上与特定点最近的一点
	Vector3	ClosestPointTo(const Vector3 &p) const;

	// 判断是否和特定的球重叠了
	bool	IntersectsSphere(const CSphere &sph) const;

	// 判断与射线的交集，返回时间参数，无碰撞，则返回>1的数
	float	RayIntersect(const Vector3 &rayOrg, const Vector3 &rayDelta,
		Vector3 *returnNormal = 0) const;

	// 判断盒子是在特定平面的哪一面
	int	ClassifyPlane(const CPlane &plane) const;

	// 动态检测和面的交集，返回时间参数
	float	IntersectPlane(const CPlane &plane, const Vector3 &dir) const;

	// 跟OBB的碰撞检测
	// vRadiiB是传入盒子的尺寸（x,y,z的一半长），假定它在自己空间原点
	// mToA是从传入盒子到我们AABB空间的变换矩阵
	bool IntersectOBB(const Vector3 &vRadii, const CMatrix44F &mToA) const;

public:
	Vector3	min;			// 左下角点
	Vector3	max;			// 右上角点
};

// 判断两个盒子是否重叠了，还可以通过参数返回重叠部分。
bool	IntersectAABBs(const AABB3 &box1, const AABB3 &box2,
					   AABB3 *boxIntersect = 0);

// 动态检测两个盒子的碰撞，返回时间参数，无碰撞，则返回>1的数
float	IntersectMovingAABB(const AABB3 &stationaryBox,
							const AABB3 &movingBox,
							const Vector3 &d);


#endif