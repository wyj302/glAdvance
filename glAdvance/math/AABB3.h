//////////////////////////////////////////////////////////////////////////
//  AABB3�࣬�������ӣ�������Ⱦ����ײ���
//
//  AABB3.h: AABB3�������
//
//////////////////////////////////////////////////////////////////////////

#ifndef __AABB3_H_INCLUDED__
#define __AABB3_H_INCLUDED__

#include "Vector3.h"

//-------------------------------------------------------------------------------
// ǰ������
//-------------------------------------------------------------------------------
class CMatrix44F;
class CSphere;
class CPlane;

//-------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------
class AABB3
{
public:
	// ��ѯ�ߴ�
	Vector3	Size() const { return max - min; }
	float	XSize() { return max.x - min.x; }
	float	YSize() { return max.y - min.y; }
	float	ZSize() { return max.z - min.z; }
	Vector3	Center() const { return (min + max) * .5f; }

	// ��ȡ���ӵ�һ������
	Vector3	Corner(int i) const;


	// ���ӵ�һЩͨ�ò���

	// �Ѻ����ÿ�
	void	Empty();

	// ����һ����
	void	Add(const Vector3 &p);

	// ����һ������
	void	Add(const AABB3 &box);

	// �任���ӣ����Ҽ����µĺ��ӣ��µĺ��ӿ��ܱ�ԭ�Ĵ�һЩ
	void	SetToTransformedBox(const AABB3 &box, const CMatrix44F &m);

	// ����/�ص����

	// �жϺ����Ƿ�Ϊ�ա�
	bool	IsEmpty() const;

	// �жϺ����Ƿ�����ض���
	bool	Contains(const Vector3 &p) const;

	// ���غ��������ض��������һ��
	Vector3	ClosestPointTo(const Vector3 &p) const;

	// �ж��Ƿ���ض������ص���
	bool	IntersectsSphere(const CSphere &sph) const;

	// �ж������ߵĽ���������ʱ�����������ײ���򷵻�>1����
	float	RayIntersect(const Vector3 &rayOrg, const Vector3 &rayDelta,
		Vector3 *returnNormal = 0) const;

	// �жϺ��������ض�ƽ�����һ��
	int	ClassifyPlane(const CPlane &plane) const;

	// ��̬������Ľ���������ʱ�����
	float	IntersectPlane(const CPlane &plane, const Vector3 &dir) const;

	// ��OBB����ײ���
	// vRadiiB�Ǵ�����ӵĳߴ磨x,y,z��һ�볤�����ٶ������Լ��ռ�ԭ��
	// mToA�ǴӴ�����ӵ�����AABB�ռ�ı任����
	bool IntersectOBB(const Vector3 &vRadii, const CMatrix44F &mToA) const;

public:
	Vector3	min;			// ���½ǵ�
	Vector3	max;			// ���Ͻǵ�
};

// �ж����������Ƿ��ص��ˣ�������ͨ�����������ص����֡�
bool	IntersectAABBs(const AABB3 &box1, const AABB3 &box2,
					   AABB3 *boxIntersect = 0);

// ��̬����������ӵ���ײ������ʱ�����������ײ���򷵻�>1����
float	IntersectMovingAABB(const AABB3 &stationaryBox,
							const AABB3 &movingBox,
							const Vector3 &d);


#endif