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
// ����8���˵��е�һ�����˵�������:
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
// ��ȡ���ӵ�һ������
//-------------------------------------------------------------------------------
Vector3 AABB3::Corner(int i) const 
{
	// ȷ��������Ч...

	assert(i >= 0);
	assert(i <= 7);

	// ������

	return Vector3(
		(i & 1) ? max.x : min.x,
		(i & 2) ? max.y : min.y,
		(i & 4) ? max.z : min.z
		);
}

//-------------------------------------------------------------------------------
// �Ѻ����ÿ�
//-------------------------------------------------------------------------------
void AABB3::Empty() 
{
	const float kBigNumber = 1e37f;
	min.x = min.y = min.z = kBigNumber;
	max.x = max.y = max.z = -kBigNumber;
}

//-------------------------------------------------------------------------------
// ����һ����
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
// ����һ������
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
// �任���ӣ����Ҽ����µĺ��ӣ��µĺ��ӿ��ܱ�ԭ�Ĵ�һЩ
//-------------------------------------------------------------------------------
void AABB3::SetToTransformedBox(const AABB3 &box, const CMatrix44F &m) 
{
	if (box.IsEmpty()) 
	{
		Empty();
		return;
	}

	// ��ƽ�Ʋ��ֿ�ʼ
	min = max = GetTranslation(m);

	// ���μ������9��Ԫ�أ������µ�AABB

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
// �жϺ����Ƿ�Ϊ��
//-------------------------------------------------------------------------------
bool AABB3::IsEmpty() const 
{
	return (min.x > max.x) || (min.y > max.y) || (min.z > max.z);
}

//-------------------------------------------------------------------------------
// �жϺ����Ƿ�����ض���
//-------------------------------------------------------------------------------
bool AABB3::Contains(const Vector3 &p) const 
{
	return
		(p.x >= min.x) && (p.x <= max.x) &&
		(p.y >= min.y) && (p.y <= max.y) &&
		(p.z >= min.z) && (p.z <= max.z);
}

//-------------------------------------------------------------------------------
// ���غ��������ض��������һ��
//-------------------------------------------------------------------------------
Vector3	AABB3::ClosestPointTo(const Vector3 &p) const
{
	// ��ÿһ�Ὣp�����򡱾��α߽��
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
// �ж��Ƿ���ض������ص���
//-------------------------------------------------------------------------------
bool AABB3::IntersectsSphere(const CSphere &sph) const
{
	// �ҵ����α߽��������������ĵ�
	Vector3 closestPoint = ClosestPointTo(sph.center);
	// �������㵽���ĵľ����Ƿ�С�ڰ뾶
	return DistanceSquared(sph.center, closestPoint) < sph.radius * sph.radius;
}

//-------------------------------------------------------------------------------
// �ж������ߵĽ���������ʱ�����������ײ���򷵻�>1����
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// �жϺ��������ض�ƽ�����һ��
//-------------------------------------------------------------------------------
int AABB3::ClassifyPlane(const CPlane &plane) const
{
	// ��鷨����������������СDֵ��������
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

	// ��ȫ��ƽ���ǰ��
	if (minD >= d)
	{
		return 1;
	}

	// ��ȫ��ƽ��ı���
	if (maxD <= d)
	{
		return -1;
	}

	// ���ƽ��
	return 0;
}

//-------------------------------------------------------------------------------
// ��̬������Ľ���������ʱ�����
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// ��OBB����ײ���
// vRadiiB�Ǵ�����ӵĳߴ磨x,y,z��һ�볤�����ٶ������Լ��ռ�ԭ��
// mToA�ǴӴ�����ӵ�����AABB�ռ�ı任����
//-------------------------------------------------------------------------------
bool AABB3::IntersectOBB(const Vector3 &bRadii, const CMatrix44F &mToA) const
{
	Vector3 p;
	mToA.GetRow(3, &p);				// �õ�OBB�����ĵ�λ��
	Vector3 aCenter = min + max;	
	aCenter *= 0.5f;				// �õ���AABB��λ��
	p -= aCenter;					// ����õ�OBB���ĵ���AABB���ĵľ���
	Vector3 aRadii = max - min;
	aRadii *= 0.5f;					// AABB�İ뾶

	float absXX, absXY, absXZ;		// X�����������
	float absYX, absYY, absYZ;		// Y�����������
	float absZX, absZY, absZZ;		// Z�����������

	const float * f = mToA.m;

	// һ������AABB���ӵ�ÿ����������ͶӰ���ж���û���ཻ

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

	// ȡ��ÿ����������x����
	absXX = fabs(f[0]);
	absYX = fabs(f[4]);
	absZX = fabs(f[8]);

	// �������������X���ϵ��ܾ���С���������ĵ�X������û���ཻ
	if (aRadii.x + bRadii.x * absXX + bRadii.y * absYX + bRadii.z * absZX - fabs(p.x)<0.0f)
		return false;

	// ȡ��ÿ�����y����
	absXY = fabs(f[1]);
	absYY = fabs(f[5]);
	absZY = fabs(f[9]);

	// �������������Y���ϵ��ܾ���С���������ĵ�Y������û���ཻ
	if (aRadii.y + bRadii.x * absXY + bRadii.y * absYY + bRadii.z * absZY - fabs(p.y)<0.0f)
		return false;

	// ȡ��ÿ�����z����
	absXZ = fabs(f[2]);
	absYZ = fabs(f[6]);
	absZZ = fabs(f[10]);

	// �������������Z���ϵ��ܾ���С���������ĵ�Z������û���ཻ
	if (aRadii.z + bRadii.x * absXZ + bRadii.y * absYZ + bRadii.z * absZZ - fabs(p.z)<0.0f)
		return false;

	// ����Ȼ����OBB����������ͶӰ���ж���û���ཻ

	// �������������OBB��X���ϵ��ܾ���С���������ĵ�X������û���ཻ
	if (aRadii.x*absXX + aRadii.y*absXY + aRadii.z*absXZ + bRadii.x - fabs(p.x*f[0] + p.y*f[1] + p.z*f[2])<0.0f)
		return false;

	// �������������OBB��Y���ϵ��ܾ���С���������ĵ�Y������û���ཻ
	if (aRadii.x*absYX + aRadii.y*absYY + aRadii.z*absYZ + bRadii.y - fabs(p.x*f[4] + p.y*f[5] + p.z*f[6])<0.0f)
		return false;		

	// �������������OBB��Z���ϵ��ܾ���С���������ĵ�Z������û���ཻ
	if (aRadii.x*absZX + aRadii.y*absZY + aRadii.z*absZZ + bRadii.z - fabs(p.x*f[8] + p.y*f[9] + p.z*f[10])<0.0f)
		return false;		

	// ������AABB��OBB��������Ĳ��������ͶӰ���ж���û���ཻ

	// ���������ӵ�X���˵�������ͶӰ���ж���û���ཻ
	// ͶӰ��Ϊ��(1, 0, 0) X (f[0], f[1], f[2]) = (0, -f[2], f[1])
	if (fabs(p.z*f[1] - p.y*f[2]) >
		aRadii.y * absXZ + aRadii.z * absXY +
		bRadii.y * absZX + bRadii.z * absYX)
		return false;

	// ͶӰ��Ϊ��(1, 0, 0) X (f[4], f[5], f[6]) = (0, -f[6], f[5])
	if (fabs(p.z*f[5] - p.y*f[6]) >
		aRadii.y * absYZ + aRadii.z * absYY +
		bRadii.x * absZX + bRadii.z * absXX)
		return false;

	// ͶӰ��Ϊ��(1, 0, 0) X (f[8], f[9], f[10]) = (0, -f[10], f[9])
	if (fabs(p.z*f[9] - p.y*f[10]) >
		aRadii.y * absZZ + aRadii.z * absZY +
		bRadii.x * absYX + bRadii.y * absXX)
		return false;

	// ͶӰ��Ϊ��(0, 1, 0) X (f[0], f[1], f[2]) = (f[2], 0, -f[0])
	if (fabs(p.x*f[2] - p.z*f[0]) >
		aRadii.x * absXZ + aRadii.z * absXX +
		bRadii.y * absZY + bRadii.z * absYY)
		return false;

	// ͶӰ��Ϊ��(0, 1, 0) X (f[4], f[5], f[6]) = (f[6], 0, -f[4])
	if (fabs(p.x*f[6] - p.z*f[4]) >
		aRadii.x * absYZ + aRadii.z * absYX +
		bRadii.x * absZY + bRadii.z * absXY)
		return false;

	// ͶӰ��Ϊ��(0, 1, 0) X (f[8], f[9], f[10]) = (f[10], 0, -f[8])
	if (fabs(p.x*f[10] - p.z*f[8]) >
		aRadii.x * absZZ + aRadii.z * absZX +
		bRadii.x * absYY + bRadii.y * absXY)
		return false;

	// ͶӰ��Ϊ��(0, 0, 1) X (f[0], f[1], f[2]) = (-f[1], f[0], 0)
	if (fabs(p.y*f[0] - p.x*f[1]) >
		aRadii.x * absXY + aRadii.y * absXX +
		bRadii.y * absZZ + bRadii.z * absYZ)
		return false;

	// ͶӰ��Ϊ��(0, 0, 1) X (f[4], f[5], f[6]) = (-f[5], f[4], 0)
	if (fabs(p.y*f[4] - p.x*f[5]) >
		aRadii.x * absYY + aRadii.y * absYX +
		bRadii.x * absZZ + bRadii.z * absXZ)
		return false;

	// ͶӰ��Ϊ��(0, 0, 1) X (f[8], f[9], f[10]) = (-f[9], f[8], 0)
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
// �ж����������Ƿ��ص��ˣ�������ͨ�����������ص����֡�
//-------------------------------------------------------------------------------
bool    IntersectAABBs(const AABB3 &box1, const AABB3 &box2, AABB3 *boxIntersect) 
{

	// ���û���ص������
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;

	// ���ص��������Ҫ�����ص����֣������һ��
	if (boxIntersect != NULL) 
	{
		boxIntersect->min.x = __max(box1.min.x, box2.min.x);
		boxIntersect->max.x = __min(box1.max.x, box2.max.x);
		boxIntersect->min.y = __max(box1.min.y, box2.min.y);
		boxIntersect->max.y = __min(box1.max.y, box2.max.y);
		boxIntersect->min.z = __max(box1.min.z, box2.min.z);
		boxIntersect->max.z = __min(box1.max.z, box2.max.z);
	}

	// �����ཻ��
    return true;
}

//-------------------------------------------------------------------------------
//  ��̬����������ӵ���ײ������ʱ�����������ײ���򷵻�>1����
//-------------------------------------------------------------------------------



// AABB�������ε��ཻ
/////   1.   �ж������ε���һ�����Ƿ�AABB����   
/////   2.   �ж������ε���һ���Ƿ���AABB�ཻ   
/////         2.1   ���ж���һ������������AABB�Ƿ��ཻ   
/////         2.2   ���ж��ཻ������Ƿ�С�������ߵı߳�   
/////   </remarks>   
/////   <param   name="aabb"></param>   
/////   <param   name="A">�����ζ���һ</param>   
/////   <param   name="B">�����ζ����</param>   
/////   <param   name="C">�����ζ�����</param>   
/////   <returns>�ཻ���</returns>   
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
/////   �Ƿ���ָ����AABB�ཻ   
/////   </summary>   
/////   <param   name="box"></param>   
/////   <returns></returns>   
//public   override   Intersection   Intersect(AxisAlignedBox   box)   
//{   
//	Intersection   result   =   base.Intersect(box);   
//	//   �����AABB�ཻ���ٽ��������μ������ײ���   
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
