//////////////////////////////////////////////////////////////////////////
//  CPlane类，平面类，碰撞和可视性检测用。
//
//  Plane.h: CPlane类的声明
//
//  Copy Rights Wonderful 2006
//////////////////////////////////////////////////////////////////////////

#ifndef __PLANE_H_INCLUDED__
#define __PLANE_H_INCLUDED__

//-------------------------------------------------------------------------------
// 前向声明
//-------------------------------------------------------------------------------
class Vector3;
class CMatrix44F;

//-------------------------------------------------------------------------------
// 平面类，碰撞和可视性检测用
//-------------------------------------------------------------------------------
class CPlane
{
public:
	// 构造函数
	CPlane() {}
	CPlane(const float *pf);
	CPlane(float fa, float fb, float fc, float fd);

	// 强制转换函数
	operator float *();
	operator const float *() const;

	// 一元操作符
	CPlane operator + () const;
	CPlane operator - () const;

	// 相等判断
	bool operator ==(const CPlane &rhs) const;
	bool operator !=(const CPlane &rhs) const;


public:
	float a, b, c, d;		// a、b、c为法线的分量，d为与原点的最短有符号距离
};

// 平面点乘向量（点），判断点在平面的哪边 
float PlaneDotCoord(const CPlane *pP, const Vector3 *pV);

// 从点和法线构造一个平面对象
CPlane *PlaneFromPointNormal(CPlane *pOut, const Vector3 *pPoint, const Vector3 *pNormal);

// 根据平面上已知的3点来求取平面
CPlane *PlaneFromPoints(CPlane *pOut, const Vector3 *pV1, const Vector3 *pV2, const Vector3 *pV3);

// 平面的规格化 
CPlane *PlaneNormalize(CPlane *pOut, const CPlane *pP);

// 平面的变换 
CPlane *PlaneTransform(CPlane *pOut, const CPlane *pP, const CMatrix44F *pM);


#endif 
