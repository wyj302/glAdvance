#include "AABB3.h"
#include "Matrix.h"
#include "Sphere.h"
#include "Plane.h"
#include <stdlib.h>

#include <windows.h>
#include <assert.h> 
//---------------------------------------------------------------------------
// AABB3::corner
//
// 返回8个端点中的一个，端点编号如下:
//
//            2                                3
//              ------------------------------
//             /|                           /|
//            / |                          / |
//           /  |                         /  |
//          /   |                        /   |
//         /    |                       /    |
//        /     |                      /     |
//       /      |                     /      |
//      /       |                    /       |
//     /        |                   /        |
//  6 /         |                7 /         |
//   /----------------------------/          |
//   |          |                 |          |
//   |          |                 |          |      +Y
//   |        0 |                 |          | 
//   |          |-----------------|----------|      |
//   |         /                  |         /  1    |
//   |        /                   |        /        |  
//   |       /                    |       /         |
//   |      /                     |      /          |     
//   |     /                      |     /           |    
//   |    /                       |    /            |   
//   |   /                        |   /             |  
//   |  /                         |  /              | 
//   | /                          | /               |
//   |/                           |/                /----------------- +X
//   ------------------------------                /
//  4                              5              /
//                                               /
// Bit 0 selects min.x vs. max.x                +Z
// Bit 1 selects min.y vs. max.y
// Bit 2 selects min.z vs. max.z
//-------------------------------------------------------------------------------
const float NO_INTERSECTION = 1e30f;
//-------------------------------------------------------------------------------
// 获取盒子的一个顶点
//-------------------------------------------------------------------------------
Vector3 AABB3::Corner(int i) const 
{
	// 确认索引有效...

	assert(i >= 0);
	assert(i <= 7);

	// 返回它

	return Vector3(
		(i & 1) ? max.x : min.x,
		(i & 2) ? max.y : min.y,
		(i & 4) ? max.z : min.z
		);
}

//-------------------------------------------------------------------------------
// 把盒子置空
//-------------------------------------------------------------------------------
void AABB3::Empty() 
{
	const float kBigNumber = 1e37f;
	min.x = min.y = min.z = kBigNumber;
	max.x = max.y = max.z = -kBigNumber;
}

//-------------------------------------------------------------------------------
// 增加一个点
//-------------------------------------------------------------------------------
void AABB3::Add(const Vector3 &p) 
{
	if (p.x < min.x) min.x = p.x;
	if (p.x > max.x) max.x = p.x;
	if (p.y < min.y) min.y = p.y;
	if (p.y > max.y) max.y = p.y;
	if (p.z < min.z) min.z = p.z;
	if (p.z > max.z) max.z = p.z;
}

//-------------------------------------------------------------------------------
// 增加一个盒子
//-------------------------------------------------------------------------------
void AABB3::Add(const AABB3 &box) 
{
	if (box.min.x < min.x) min.x = box.min.x;
	if (box.max.x > max.x) max.x = box.max.x;
	if (box.min.y < min.y) min.y = box.min.y;
	if (box.max.y > max.y) max.y = box.max.y;
	if (box.min.z < min.z) min.z = box.min.z;
	if (box.max.z > max.z) max.z = box.max.z;
}

//-------------------------------------------------------------------------------
// 变换盒子，并且计算新的盒子，新的盒子可能比原的大一些
//-------------------------------------------------------------------------------
void AABB3::SetToTransformedBox(const AABB3 &box, const CMatrix44F &m) 
{
	if (box.IsEmpty()) 
	{
		Empty();
		return;
	}

	// 从平移部分开始
	min = max = GetTranslation(m);

	// 依次检查矩阵的9个元素，计算新的AABB

	if (m.m[0] > 0.0f) 
	{
		min.x += m.m[0] * box.min.x; max.x += m.m[0] * box.max.x;
	} 
	else 
	{
		min.x += m.m[0] * box.max.x; max.x += m.m[0] * box.min.x;
	}

	if (m.m[1] > 0.0f) 
	{
		min.y += m.m[1] * box.min.x; max.y += m.m[1] * box.max.x;
	} 
	else 
	{
		min.y += m.m[1] * box.max.x; max.y += m.m[1] * box.min.x;
	}

	if (m.m[2] > 0.0f) 
	{
		min.z += m.m[2] * box.min.x; max.z += m.m[2] * box.max.x;
	} 
	else 
	{
		min.z += m.m[2] * box.max.x; max.z += m.m[2] * box.min.x;
	}

	if (m.m[4] > 0.0f) 
	{
		min.x += m.m[4] * box.min.y; max.x += m.m[4] * box.max.y;
	} 
	else 
	{
		min.x += m.m[4] * box.max.y; max.x += m.m[4] * box.min.y;
	}

	if (m.m[5] > 0.0f) 
	{
		min.y += m.m[5] * box.min.y; max.y += m.m[5] * box.max.y;
	} 
	else 
	{
		min.y += m.m[5] * box.max.y; max.y += m.m[5] * box.min.y;
	}

	if (m.m[6] > 0.0f) 
	{
		min.z += m.m[6] * box.min.y; max.z += m.m[6] * box.max.y;
	} 
	else 
	{
		min.z += m.m[6] * box.max.y; max.z += m.m[6] * box.min.y;
	}

	if (m.m[8] > 0.0f) 
	{
		min.x += m.m[8] * box.min.z; max.x += m.m[8] * box.max.z;
	} 
	else 
	{
		min.x += m.m[8] * box.max.z; max.x += m.m[8] * box.min.z;
	}

	if (m.m[9] > 0.0f) 
	{
		min.y += m.m[9] * box.min.z; max.y += m.m[9] * box.max.z;
	} 
	else 
	{
		min.y += m.m[9] * box.max.z; max.y += m.m[9] * box.min.z;
	}

	if (m.m[10] > 0.0f) 
	{
		min.z += m.m[10] * box.min.z; max.z += m.m[10] * box.max.z;
	} 
	else 
	{
		min.z += m.m[10] * box.max.z; max.z += m.m[10] * box.min.z;
	}
}

//-------------------------------------------------------------------------------
// 判断盒子是否为空
//-------------------------------------------------------------------------------
bool AABB3::IsEmpty() const 
{
	return (min.x > max.x) || (min.y > max.y) || (min.z > max.z);
}

//-------------------------------------------------------------------------------
// 判断盒子是否包含特定点
//-------------------------------------------------------------------------------
bool AABB3::Contains(const Vector3 &p) const 
{
	return
		(p.x >= min.x) && (p.x <= max.x) &&
		(p.y >= min.y) && (p.y <= max.y) &&
		(p.z >= min.z) && (p.z <= max.z);
}

//-------------------------------------------------------------------------------
// 返回盒子上与特定点最近的一点
//-------------------------------------------------------------------------------
Vector3	AABB3::ClosestPointTo(const Vector3 &p) const
{
	// 在每一轴将p“推向”矩形边界框
	Vector3 r;

	if (p.x < min.x)
	{
		r.x = min.x;
	}
	else if (p.x > max.x)
	{
		r.x = max.x;
	}
	else
	{
		r.x = p.x;
	}

	if (p.y < min.y)
	{
		r.y = min.y;
	}
	else if (p.y > max.y)
	{
		r.y = max.y;
	}
	else
	{
		r.y = p.y;
	}

	if (p.z < min.z)
	{
		r.z = min.z;
	}
	else if (p.z > max.z)
	{
		r.z = max.z;
	}
	else
	{
		r.z = p.z;
	}

	return r;

}

//-------------------------------------------------------------------------------
// 判断是否和特定的球重叠了
//-------------------------------------------------------------------------------
bool AABB3::IntersectsSphere(const CSphere &sph) const
{
	// 找到矩形边界框上离球心最近的点
	Vector3 closestPoint = ClosestPointTo(sph.center);
	// 检查最近点到球心的距离是否小于半径
	return DistanceSquared(sph.center, closestPoint) < sph.radius * sph.radius;
}

//-------------------------------------------------------------------------------
// 判断与射线的交集，返回时间参数，无碰撞，则返回>1的数
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// 判断盒子是在特定平面的哪一面
//-------------------------------------------------------------------------------
int AABB3::ClassifyPlane(const CPlane &plane) const
{
	// 检查法向量，计算最大和最小D值，即距离
	Vector3 n(plane.a, plane.b, plane.c);
	float d = plane.d;

	float minD, maxD;

	if (n.x > 0.f)
	{
		minD = n.x * min.x; 
		maxD = n.x * max.x;
	}
	else
	{
		minD = n.x * max.x;
		maxD = n.x * min.x;
	}

	if (n.y > 0.f)
	{
		minD += n.y * min.y; 
		maxD += n.y * max.y;
	}
	else
	{
		minD += n.y * max.y;
		maxD += n.y * min.y;
	}

	if (n.z > 0.f)
	{
		minD += n.z * min.z; 
		maxD += n.z * max.z;
	}
	else
	{
		minD += n.z * max.z;
		maxD += n.z * min.z;
	}

	// 完全在平面的前面
	if (minD >= d)
	{
		return 1;
	}

	// 完全在平面的背面
	if (maxD <= d)
	{
		return -1;
	}

	// 横跨平面
	return 0;
}

//-------------------------------------------------------------------------------
// 动态检测和面的交集，返回时间参数
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// 跟OBB的碰撞检测
// vRadiiB是传入盒子的尺寸（x,y,z的一半长），假定它在自己空间原点
// mToA是从传入盒子到我们AABB空间的变换矩阵
//-------------------------------------------------------------------------------
bool AABB3::IntersectOBB(const Vector3 &bRadii, const CMatrix44F &mToA) const
{
	Vector3 p;
	mToA.GetRow(3, &p);				// 得到OBB的中心的位置
	Vector3 aCenter = min + max;	
	aCenter *= 0.5f;				// 得到本AABB的位置
	p -= aCenter;					// 这个得到OBB中心到本AABB中心的距离
	Vector3 aRadii = max - min;
	aRadii *= 0.5f;					// AABB的半径

	float absXX, absXY, absXZ;		// X轴的向量方向
	float absYX, absYY, absYZ;		// Y轴的向量方向
	float absZX, absZY, absZZ;		// Z轴的向量方向

	const float * f = mToA.m;

	// 一，先在AABB盒子的每个坐标轴上投影，判断有没有相交

	//                     ***
	//                    *    *
	//                   *      *
	// *************     **   *  * 
	// *           *        **  *
	// *      *    *          **  
	// *           *         
	// *************  bRadii.x * absXX                    
	//                    \/          
	//      |aRadii.x|   |   | | <---bRadii.y * absYX
	//-------------------------------------------------------
    //      |      p.x       |

	// 取出每个轴向量的x分量
	absXX = fabs(f[0]);
	absYX = fabs(f[4]);
	absZX = fabs(f[8]);

	// 如果两个盒子在X轴上的总距离小于两个中心的X距离则没有相交
	if (aRadii.x + bRadii.x * absXX + bRadii.y * absYX + bRadii.z * absZX - fabs(p.x)<0.0f)
		return false;

	// 取出每个轴的y分量
	absXY = fabs(f[1]);
	absYY = fabs(f[5]);
	absZY = fabs(f[9]);

	// 如果两个盒子在Y轴上的总距离小于两个中心的Y距离则没有相交
	if (aRadii.y + bRadii.x * absXY + bRadii.y * absYY + bRadii.z * absZY - fabs(p.y)<0.0f)
		return false;

	// 取出每个轴的z分量
	absXZ = fabs(f[2]);
	absYZ = fabs(f[6]);
	absZZ = fabs(f[10]);

	// 如果两个盒子在Z轴上的总距离小于两个中心的Z距离则没有相交
	if (aRadii.z + bRadii.x * absXZ + bRadii.y * absYZ + bRadii.z * absZZ - fabs(p.z)<0.0f)
		return false;

	// 二、然后在OBB的坐标轴上投影，判断有没有相交

	// 如果两个盒子在OBB的X轴上的总距离小于两个中心的X距离则没有相交
	if (aRadii.x*absXX + aRadii.y*absXY + aRadii.z*absXZ + bRadii.x - fabs(p.x*f[0] + p.y*f[1] + p.z*f[2])<0.0f)
		return false;

	// 如果两个盒子在OBB的Y轴上的总距离小于两个中心的Y距离则没有相交
	if (aRadii.x*absYX + aRadii.y*absYY + aRadii.z*absYZ + bRadii.y - fabs(p.x*f[4] + p.y*f[5] + p.z*f[6])<0.0f)
		return false;		

	// 如果两个盒子在OBB的Z轴上的总距离小于两个中心的Z距离则没有相交
	if (aRadii.x*absZX + aRadii.y*absZY + aRadii.z*absZZ + bRadii.z - fabs(p.x*f[8] + p.y*f[9] + p.z*f[10])<0.0f)
		return false;		

	// 三、在AABB与OBB的三个轴的叉乘向量上投影，判断有没有相交

	// 在两个盒子的X轴叉乘的向量上投影，判断有没有相交
	// 投影轴为：(1, 0, 0) X (f[0], f[1], f[2]) = (0, -f[2], f[1])
	if (fabs(p.z*f[1] - p.y*f[2]) >
		aRadii.y * absXZ + aRadii.z * absXY +
		bRadii.y * absZX + bRadii.z * absYX)
		return false;

	// 投影轴为：(1, 0, 0) X (f[4], f[5], f[6]) = (0, -f[6], f[5])
	if (fabs(p.z*f[5] - p.y*f[6]) >
		aRadii.y * absYZ + aRadii.z * absYY +
		bRadii.x * absZX + bRadii.z * absXX)
		return false;

	// 投影轴为：(1, 0, 0) X (f[8], f[9], f[10]) = (0, -f[10], f[9])
	if (fabs(p.z*f[9] - p.y*f[10]) >
		aRadii.y * absZZ + aRadii.z * absZY +
		bRadii.x * absYX + bRadii.y * absXX)
		return false;

	// 投影轴为：(0, 1, 0) X (f[0], f[1], f[2]) = (f[2], 0, -f[0])
	if (fabs(p.x*f[2] - p.z*f[0]) >
		aRadii.x * absXZ + aRadii.z * absXX +
		bRadii.y * absZY + bRadii.z * absYY)
		return false;

	// 投影轴为：(0, 1, 0) X (f[4], f[5], f[6]) = (f[6], 0, -f[4])
	if (fabs(p.x*f[6] - p.z*f[4]) >
		aRadii.x * absYZ + aRadii.z * absYX +
		bRadii.x * absZY + bRadii.z * absXY)
		return false;

	// 投影轴为：(0, 1, 0) X (f[8], f[9], f[10]) = (f[10], 0, -f[8])
	if (fabs(p.x*f[10] - p.z*f[8]) >
		aRadii.x * absZZ + aRadii.z * absZX +
		bRadii.x * absYY + bRadii.y * absXY)
		return false;

	// 投影轴为：(0, 0, 1) X (f[0], f[1], f[2]) = (-f[1], f[0], 0)
	if (fabs(p.y*f[0] - p.x*f[1]) >
		aRadii.x * absXY + aRadii.y * absXX +
		bRadii.y * absZZ + bRadii.z * absYZ)
		return false;

	// 投影轴为：(0, 0, 1) X (f[4], f[5], f[6]) = (-f[5], f[4], 0)
	if (fabs(p.y*f[4] - p.x*f[5]) >
		aRadii.x * absYY + aRadii.y * absYX +
		bRadii.x * absZZ + bRadii.z * absXZ)
		return false;

	// 投影轴为：(0, 0, 1) X (f[8], f[9], f[10]) = (-f[9], f[8], 0)
	if (fabs(p.y*f[8] - p.x*f[9]) >
		aRadii.x * absZY + aRadii.y * absZX +
		bRadii.x * absYZ + bRadii.y * absXZ)
		return false;

	return true;
}
//---------------------------------------------------------------------------
// Parametric intersection with a ray.  Returns parametric point
// of intsersection in range 01 or a really big number (>1) if no
// intersection.
//
// From "Fast Ray-Box Intersection," by Woo in Graphics Gems I, page 395.
//
// See 12.9.11
//---------------------------------------------------------------------------
float AABB3::RayIntersect(const Vector3& ray_org, // origin of the ray
	const Vector3& ray_delta,        // length and direction of the ray
	Vector3* return_normal) const    // optionally, the normal is returned
{
	// Check for point inside box, trivial reject, and determine parametric distance to each front face.
	bool inside = true;

	float xt, xn = 0.0f;

	if (ray_org.x < min.x)
	{
		xt = min.x - ray_org.x;
		if (xt > ray_delta.x)
			return NO_INTERSECTION;

		xt /= ray_delta.x;
		inside = false;
		xn = -1.0f;
	}
	else if (ray_org.x > max.x)
	{
		xt = max.x - ray_org.x;
		if (xt < ray_delta.x)
			return NO_INTERSECTION;

		xt /= ray_delta.x;
		inside = false;
		xn = 1.0f;
	}
	else
		xt = -1.0f;

	float yt, yn = 0.0f;

	if (ray_org.y < min.y)
	{
		yt = min.y - ray_org.y;
		if (yt > ray_delta.y)
			return NO_INTERSECTION;

		yt /= ray_delta.y;
		inside = false;
		yn = -1.0f;
	}
	else if (ray_org.y > max.y)
	{
		yt = max.y - ray_org.y;
		if (yt < ray_delta.y)
			return NO_INTERSECTION;

		yt /= ray_delta.y;
		inside = false;
		yn = 1.0f;
	}
	else
		yt = -1.0f;

	float zt, zn = 0.0f;

	if (ray_org.z < min.z)
	{
		zt = min.z - ray_org.z;
		if (zt > ray_delta.z)
			return NO_INTERSECTION;

		zt /= ray_delta.z;
		inside = false;
		zn = -1.0f;
	}
	else if (ray_org.z > max.z)
	{
		zt = max.z - ray_org.z;
		if (zt < ray_delta.z)
			return NO_INTERSECTION;

		zt /= ray_delta.z;
		inside = false;
		zn = 1.0f;
	}
	else
		zt = -1.0f;

	// Inside box?
	if (inside)
	{
		if (return_normal != NULL)
		{
			*return_normal = -ray_delta;
			return_normal->Normalize();
		}

		return 0.0f;
	}

	// Select farthest plane - this is the plane of intersection.

	int which = 0;
	float t = xt;

	if (yt > t)
	{
		which = 1;
		t = yt;
	}

	if (zt > t)
	{
		which = 2;
		t = zt;
	}

	switch (which)
	{
	case 0: // intersect with yz plane
	{
		float y = ray_org.y + ray_delta.y * t;

		if (y < min.y || y > max.y)
			return NO_INTERSECTION;

		float z = ray_org.z + ray_delta.z * t;
		if (z < min.z || z > max.z)
			return NO_INTERSECTION;

		if (return_normal != NULL)
		{
			return_normal->x = xn;
			return_normal->y = 0.0f;
			return_normal->z = 0.0f;
		}
	}
	break;

	case 1: // intersect with xz plane
	{
		float x = ray_org.x + ray_delta.x * t;
		if (x < min.x || x > max.x)
			return NO_INTERSECTION;

		float z = ray_org.z + ray_delta.z * t;
		if (z < min.z || z > max.z)
			return NO_INTERSECTION;

		if (return_normal != NULL)
		{
			return_normal->x = 0.0f;
			return_normal->y = yn;
			return_normal->z = 0.0f;
		}

	}
	break;

	case 2: // intersect with xy plane
	{
		float x = ray_org.x + ray_delta.x * t;
		if (x < min.x || x > max.x)
			return NO_INTERSECTION;

		float y = ray_org.y + ray_delta.y * t;
		if (y < min.y || y > max.y)
			return NO_INTERSECTION;

		if (return_normal != NULL)
		{
			return_normal->x = 0.0f;
			return_normal->y = 0.0f;
			return_normal->z = zn;
		}
	}
	break;
	}

	// Return parametric point of intersection
	return t;
}
//-------------------------------------------------------------------------------
// 判断两个盒子是否重叠了，还可以通过参数返回重叠部分。
//-------------------------------------------------------------------------------
bool    IntersectAABBs(const AABB3 &box1, const AABB3 &box2, AABB3 *boxIntersect) 
{

	// 检测没有重叠的情况
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;

	// 有重叠，如果需要返回重叠部分，则计算一下
	if (boxIntersect != NULL) 
	{
		boxIntersect->min.x = __max(box1.min.x, box2.min.x);
		boxIntersect->max.x = __min(box1.max.x, box2.max.x);
		boxIntersect->min.y = __max(box1.min.y, box2.min.y);
		boxIntersect->max.y = __min(box1.max.y, box2.max.y);
		boxIntersect->min.z = __max(box1.min.z, box2.min.z);
		boxIntersect->max.z = __min(box1.max.z, box2.max.z);
	}

	// 返回相交了
    return true;
}

//-------------------------------------------------------------------------------
//  动态检测两个盒子的碰撞，返回时间参数，无碰撞，则返回>1的数
//-------------------------------------------------------------------------------



// AABB与三角形的相交
/////   1.   判断三角形的任一顶点是否被AABB包含   
/////   2.   判断三角形的任一边是否与AABB相交   
/////         2.1   先判断任一边所在射线与AABB是否相交   
/////         2.2   再判断相交点距离是否小于这条边的边长   
/////   </remarks>   
/////   <param   name="aabb"></param>   
/////   <param   name="A">三角形顶点一</param>   
/////   <param   name="B">三角形顶点二</param>   
/////   <param   name="C">三角形顶点三</param>   
/////   <returns>相交情况</returns>   
//public   static   bool   Intersects(AxisAlignedBox   aabb,   Vector3   A,   Vector3   B,   Vector3   C)   
//{   
//	if (aabb.IsContainVector(A)   ||   aabb.IsContainVector(B)||   aabb.IsContainVector(C))   
//		return   true;   
//
//	Vector3[]   vertexs   =   new   Vector3[]{A,B,C};   
//	Vector3[]   edges   =   new   Vector3[]{B   -   A,   C   -   B,   A   -   C};   
//	Ray   ray   =   null; IntersectResult   result;   
//
//	for(int   i   =   0;   i   <   3;   i++)   
//	{   
//		ray   =   new   Ray(vertexs[i],   edges[i]);   
//		result   =   Intersects(ray,   aabb);   
//		if(result.Hit   &&   result.Distance   *   result.Distance   <   edges[i].LengthSq())   
//			return   true;   
//	}   
//	return   false;   
//}   

/////   <summary>   
/////   是否与指定的AABB相交   
/////   </summary>   
/////   <param   name="box"></param>   
/////   <returns></returns>   
//public   override   Intersection   Intersect(AxisAlignedBox   box)   
//{   
//	Intersection   result   =   base.Intersect(box);   
//	//   如果与AABB相交，再进行三角形级别的碰撞检测   
//	if(result   !=   Intersection.None)   
//	{   
//		Matrix   M   =   this.ParentNodeFullTransform;   
//		Vector3   A,B,C;   
//		for(int   i   =   0;   i   <   this.mesh.Geom.TriangleCount;   i++)   
//		{   
//			A   =   Vector3.TransformCoordinate(this.mesh.Geom.Vertices[this.mesh.Geom.TriangleIndex[i   *   3]],   M);   
//			B   =   Vector3.TransformCoordinate(this.mesh.Geom.Vertices[this.mesh.Geom.TriangleIndex[i   *   3   +   1]],   M);   
//			C   =   Vector3.TransformCoordinate(this.mesh.Geom.Vertices[this.mesh.Geom.TriangleIndex[i   *   3   +   2]],   M);   
//			if(Smart.Maths.MathUtil.Intersects(box,   A,   B,   C))   
//				return   Intersection.Partial;   
//		}   
//	}   
//	return   Intersection.None;   
//}   
