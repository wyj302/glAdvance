
#include "Plane.h"
#include "Vector3.h"
#include "Matrix.h"

//-------------------------------------------------------------------------------
// 构造函数
//-------------------------------------------------------------------------------
CPlane::CPlane(const float *pf)
{
	if (!pf)
		return;

	a = pf[0];
	b = pf[1];
	c = pf[2];
	d = pf[3];
}

//-------------------------------------------------------------------------------
// 构造函数
//-------------------------------------------------------------------------------
CPlane::CPlane(float fa, float fb, float fc, float fd)
{
	a = fa;
	b = fb;
	c = fc;
	d = fd;
}

//-------------------------------------------------------------------------------
// 强制转换函数
//-------------------------------------------------------------------------------
CPlane::operator float *()
{
	return (float *) &a;
}

//-------------------------------------------------------------------------------
// 强制转换函数
//-------------------------------------------------------------------------------
CPlane::operator const float *() const
{
	return (const float *) &a;
}

//-------------------------------------------------------------------------------
// 一元操作符
//-------------------------------------------------------------------------------
CPlane CPlane::operator + () const
{
	return *this;
}

//-------------------------------------------------------------------------------
// 一元操作符
//-------------------------------------------------------------------------------
CPlane CPlane::operator - () const
{
	return CPlane(-a, -b, -c, -d);
}

//-------------------------------------------------------------------------------
// 相等判断
//-------------------------------------------------------------------------------
bool CPlane::operator == (const CPlane& p) const
{
	return a == p.a && b == p.b && c == p.c && d == p.d;
}

//-------------------------------------------------------------------------------
// 相等判断
//-------------------------------------------------------------------------------
bool CPlane::operator != (const CPlane& p) const
{
	return a != p.a || b != p.b || c != p.c || d != p.d;
}

//-------------------------------------------------------------------------------
// 平面点乘向量（点），判断点在平面的哪边
//-------------------------------------------------------------------------------
float PlaneDotCoord(const CPlane *pP, const Vector3 *pV)
{
	if (!pP || !pV)
		return 0.0f;

	return pP->a * pV->x + pP->b * pV->y + pP->c * pV->z + pP->d;
}

//-------------------------------------------------------------------------------
// 从点和法线构造一个平面对象
//-------------------------------------------------------------------------------
CPlane* PlaneFromPointNormal(CPlane *pOut, const Vector3 *pPoint, const Vector3 *pNormal)
{
	return nullptr;
}

//-------------------------------------------------------------------------------
// 根据平面上已知的3点来求取平面
//-------------------------------------------------------------------------------
CPlane* PlaneFromPoints(CPlane *pOut, const Vector3 *pV1, const Vector3 *pV2, const Vector3 *pV3)
{
	return nullptr;
}

//-------------------------------------------------------------------------------
// 平面的规格化
//-------------------------------------------------------------------------------
CPlane* PlaneNormalize(CPlane *pOut, const CPlane *pP)
{
	float magnitude = (float)sqrt(pP->a * pP->a + pP->b * pP->b + pP->c * pP->c);
	pOut->a = pP->a / magnitude;
	pOut->b = pP->b / magnitude;
	pOut->c = pP->c / magnitude;
	pOut->d = pP->d / magnitude;

	return pOut;
}

//-------------------------------------------------------------------------------
// 平面的变换
//-------------------------------------------------------------------------------
CPlane* PlaneTransform(CPlane *pOut, const CPlane *pP, const CMatrix44F *pM)
{
	return nullptr;
}

