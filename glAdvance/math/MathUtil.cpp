/////////////////////////////////////////////////////////////////////////////
//
// 3D Math Primer for Games and Graphics Development
//
// MathUtil.cpp - Miscellaneous math utilities
//
// Visit gamemath.com for the latest version of this file.
//
/////////////////////////////////////////////////////////////////////////////
#include "MathUtil.h"
#include "Vector3.h"

const Vector3 kZeroVector(0.0f, 0.0f, 0.0f);

//-------------------------------------------------------------------------------
// 把角度控制在 -pi 和 pi 之间（通过加减2pi）
//-------------------------------------------------------------------------------
float wrapPi(float theta) 
{
	theta += kPi;
	theta -= floor(theta * k1Over2Pi) * k2Pi;
	theta -= kPi;
	return theta;
}

//---------------------------------------------------------------------------
// safeAcos
//
// Same as acos(x), but if x is out of range, it is "clamped" to the nearest
// valid value.  The value returned is in range 0...pi, the same as the
// standard C acos() function

//-------------------------------------------------------------------------------
// 安全的acos函数，处理了x超出范围的情况
//-------------------------------------------------------------------------------
float safeAcos(float x) 
{
	// Check limit conditions

	if (x <= -1.0f) 
	{
		return kPi;
	}

	if (x >= 1.0f) 
	{
		return 0.0f;
	}

	// Value is in the domain - use standard C function

	return acos(x);
}
