//////////////////////////////////////////////////////////////////////////
//  CSphere�࣬���࣬������Ⱦ����ײ���
//
//  Sphere.h: Sphere�������
//
//  Copy Rights Wonderful 2006
//////////////////////////////////////////////////////////////////////////

#ifndef __SPHERE_H_INCLUDED__
#define __SPHERE_H_INCLUDED__

#include "Vector3.h"

//-------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------
class CSphere
{ 
public:
	// ���캯��
	CSphere() { }
	CSphere(const Vector3& pos, float rad) : center(pos), radius(rad) { if (radius < 0.0f) radius = 0.0f; }

	// �ÿ�
	void Empty();
	// �Ƿ�Ϊ��
	bool IsEmpty() const;

	// �Ƿ����ĳ����
	bool Contains(const Vector3& point) const;
	// �Ƿ����ĳ����
	bool Contains(const CSphere& sphere) const;

public:
	Vector3  center;		// ���ĵ�
	float    radius;		// �뾶
};

// �������Ƿ��ཻ
bool IntersectSpheres(const CSphere &sphere1, const CSphere &sphere2);


#endif 