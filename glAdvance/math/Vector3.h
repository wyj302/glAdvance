/////////////////////////////////////////////////////////////////////////////
//
// Vector3�ࣺ�����������
//
// Vector3.h - Declarations for 3D vector class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __VECTOR3_H_INCLUDED__
#define __VECTOR3_H_INCLUDED__
#include <math.h>
//-------------------------------------------------------------------------------
// 3D������
//-------------------------------------------------------------------------------
class Vector3 
{
public:
	// ���캯��
	Vector3() {}
	Vector3(const Vector3 &a) : x(a.x), y(a.y), z(a.z) {}
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	// ��ֵ����
	Vector3 &operator =(const Vector3 &a) { x = a.x; y = a.y; z = a.z; return *this; }

	// ����ж�
	bool operator ==(const Vector3 &a) const { return x==a.x && y==a.y && z==a.z; }
	bool operator !=(const Vector3 &a) const { return x!=a.x || y!=a.y || z!=a.z; }

	// �Ƿ�Ϊ������
	void Zero() { x = y = z = 0.0f; }

	// ������
	Vector3 operator -() const { return Vector3(-x,-y,-z); }
	
	// �����Ӽ�����
	Vector3 operator +(const Vector3 &a) const { return Vector3(x + a.x, y + a.y, z + a.z); }
	Vector3 operator -(const Vector3 &a) const { return Vector3(x - a.x, y - a.y, z - a.z); }

	// �ͳ����ĳ˳�����
	Vector3 operator *(float a) const { return Vector3(x*a, y*a, z*a); }
	Vector3 operator /(float a) const { float oneOverA = 1.0f / a; return Vector3(x*oneOverA, y*oneOverA, z*oneOverA); }

	// ����/������
	Vector3 &operator +=(const Vector3 &a) { x += a.x; y += a.y; z += a.z; return *this; }
	Vector3 &operator -=(const Vector3 &a) { x -= a.x; y -= a.y; z -= a.z; return *this; }

	// �ͳ�������/������
	Vector3 &operator *=(float a) { x *= a; y *= a; z *= a; return *this; }
	Vector3 &operator /=(float a) { float oneOverA = 1.0f / a; x *= oneOverA; y *= oneOverA; z *= oneOverA; return *this; }

	// ��׼���������
	void	Normalize() 
	{
		float magSq = x*x + y*y + z*z;
		
		if (magSq > 0.0f) 
		{ 
			float oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

	// �������
	float operator *(const Vector3 &a) const { return x*a.x + y*a.y + z*a.z; }

public:
	float x, y, z;	// ������������
};

/////////////////////////////////////////////////////////////////////////////
//
// �ǳ�Ա����
//
/////////////////////////////////////////////////////////////////////////////

// ���������ĳ���
inline float VectorMag(const Vector3 &a) 
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// �������
inline Vector3 CrossProduct(const Vector3 &a, const Vector3 &b) 
{
	return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

// �����������ĳ˷���
inline Vector3 operator *(float k, const Vector3 &v) 
{
	return Vector3(k*v.x, k*v.y, k*v.z);
}

// ���������ľ���
inline float Distance(const Vector3 &a, const Vector3 &b) 
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

// ���������ƽ��
inline float DistanceSquared(const Vector3 &a, const Vector3 &b) 
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return dx*dx + dy*dy + dz*dz;
}

/////////////////////////////////////////////////////////////////////////////
//
// ȫ�ֱ���
//
/////////////////////////////////////////////////////////////////////////////

// ������
extern const Vector3 kZeroVector;

#endif 
