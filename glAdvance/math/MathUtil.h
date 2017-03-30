/////////////////////////////////////////////////////////////////////////////
//
// 各种实用的数学常量和函数
//
// MathUtil.h - Declarations for miscellaneous math utilities
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MATHUTIL_H_INCLUDED__
#define __MATHUTIL_H_INCLUDED__

//-------------------------------------------------------------------------------
// 定义一些 pi 相关的常量
//-------------------------------------------------------------------------------
const float kPi = 3.14159265f;			// PI
const float k2Pi = kPi * 2.0f;			// 2PI
const float kPiOver2 = kPi / 2.0f;		// PI/2
const float k1OverPi = 1.0f / kPi;		// 1/PI
const float k1Over2Pi = 1.0f / k2Pi;	// 1/2PI
const float kPiOver180 = kPi / 180.0f;	// PI/180
const float k180OverPi = 180.0f / kPi;	// 180/PI

// 把角度控制在 -pi 和 pi 之间（通过加减2pi）
extern float wrapPi(float theta);
// 安全的acos函数，处理了x超出范围的情况
extern float safeAcos(float x);

//-------------------------------------------------------------------------------
// 转换角度和弧度。
//-------------------------------------------------------------------------------
// 1.角度到弧度
inline float	degToRad(float deg) { return deg * kPiOver180; }
// 2.弧度到角度
inline float	radToDeg(float rad) { return rad * k180OverPi; }

// 同时计算sin和con的值，在有些平台会有优化的方法。
inline void sinCos(float *returnSin, float *returnCos, float theta) 
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}
 
//-------------------------------------------------------------------------------
// 从视野角度转换到zoom，FOV角度为弧度
//-------------------------------------------------------------------------------
inline float	fovToZoom(float fov) { return 1.0f / tan(fov * .5f); }
inline float	zoomToFov(float zoom) { return 2.0f * atan(1.0f / zoom); }

#endif 
