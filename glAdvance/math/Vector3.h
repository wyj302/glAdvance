/////////////////////////////////////////////////////////////////////////////
//
// Vector3类：点和向量的类
//
// Vector3.h - Declarations for 3D vector class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __VECTOR3_H_INCLUDED__
#define __VECTOR3_H_INCLUDED__
#include <math.h>
//-------------------------------------------------------------------------------
// 3D向量类
//-------------------------------------------------------------------------------
class Vector3 
{
public:
	// 构造函数
	Vector3() {}
	Vector3(const Vector3 &a) : x(a.x), y(a.y), z(a.z) {}
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	// 赋值函数
	Vector3 &operator =(const Vector3 &a) { x = a.x; y = a.y; z = a.z; return *this; }

	// 相等判断
	bool operator ==(const Vector3 &a) const { return x==a.x && y==a.y && z==a.z; }
	bool operator !=(const Vector3 &a) const { return x!=a.x || y!=a.y || z!=a.z; }

	// 是否为零向量
	void Zero() { x = y = z = 0.0f; }

	// 负向量
	Vector3 operator -() const { return Vector3(-x,-y,-z); }
	
	// 向量加减运算
	Vector3 operator +(const Vector3 &a) const { return Vector3(x + a.x, y + a.y, z + a.z); }
	Vector3 operator -(const Vector3 &a) const { return Vector3(x - a.x, y - a.y, z - a.z); }

	// 和常数的乘除运算
	Vector3 operator *(float a) const { return Vector3(x*a, y*a, z*a); }
	Vector3 operator /(float a) const { float oneOverA = 1.0f / a; return Vector3(x*oneOverA, y*oneOverA, z*oneOverA); }

	// 连加/减函数
	Vector3 &operator +=(const Vector3 &a) { x += a.x; y += a.y; z += a.z; return *this; }
	Vector3 &operator -=(const Vector3 &a) { x -= a.x; y -= a.y; z -= a.z; return *this; }

	// 和常数连乘/除函数
	Vector3 &operator *=(float a) { x *= a; y *= a; z *= a; return *this; }
	Vector3 &operator /=(float a) { float oneOverA = 1.0f / a; x *= oneOverA; y *= oneOverA; z *= oneOverA; return *this; }

	// 标准化这个向量
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

	// 向量点乘
	float operator *(const Vector3 &a) const { return x*a.x + y*a.y + z*a.z; }

public:
	float x, y, z;	// 三个分量坐标
};

/////////////////////////////////////////////////////////////////////////////
//
// 非成员函数
//
/////////////////////////////////////////////////////////////////////////////

// 计算向量的长度
inline float VectorMag(const Vector3 &a) 
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// 向量叉乘
inline Vector3 CrossProduct(const Vector3 &a, const Vector3 &b) 
{
	return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

// 常数和向量的乘法。
inline Vector3 operator *(float k, const Vector3 &v) 
{
	return Vector3(k*v.x, k*v.y, k*v.z);
}

// 计算两点间的距离
inline float Distance(const Vector3 &a, const Vector3 &b) 
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

// 两点间距离的平方
inline float DistanceSquared(const Vector3 &a, const Vector3 &b) 
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return dx*dx + dy*dy + dz*dz;
}

/////////////////////////////////////////////////////////////////////////////
//
// 全局变量
//
/////////////////////////////////////////////////////////////////////////////

// 零向量
extern const Vector3 kZeroVector;

#endif 
