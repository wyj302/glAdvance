/////////////////////////////////////////////////////////////////////////////
//
// ����ʵ�õ���ѧ�����ͺ���
//
// MathUtil.h - Declarations for miscellaneous math utilities
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MATHUTIL_H_INCLUDED__
#define __MATHUTIL_H_INCLUDED__

//-------------------------------------------------------------------------------
// ����һЩ pi ��صĳ���
//-------------------------------------------------------------------------------
const float kPi = 3.14159265f;			// PI
const float k2Pi = kPi * 2.0f;			// 2PI
const float kPiOver2 = kPi / 2.0f;		// PI/2
const float k1OverPi = 1.0f / kPi;		// 1/PI
const float k1Over2Pi = 1.0f / k2Pi;	// 1/2PI
const float kPiOver180 = kPi / 180.0f;	// PI/180
const float k180OverPi = 180.0f / kPi;	// 180/PI

// �ѽǶȿ����� -pi �� pi ֮�䣨ͨ���Ӽ�2pi��
extern float wrapPi(float theta);
// ��ȫ��acos������������x������Χ�����
extern float safeAcos(float x);

//-------------------------------------------------------------------------------
// ת���ǶȺͻ��ȡ�
//-------------------------------------------------------------------------------
// 1.�Ƕȵ�����
inline float	degToRad(float deg) { return deg * kPiOver180; }
// 2.���ȵ��Ƕ�
inline float	radToDeg(float rad) { return rad * k180OverPi; }

// ͬʱ����sin��con��ֵ������Щƽ̨�����Ż��ķ�����
inline void sinCos(float *returnSin, float *returnCos, float theta) 
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}
 
//-------------------------------------------------------------------------------
// ����Ұ�Ƕ�ת����zoom��FOV�Ƕ�Ϊ����
//-------------------------------------------------------------------------------
inline float	fovToZoom(float fov) { return 1.0f / tan(fov * .5f); }
inline float	zoomToFov(float zoom) { return 2.0f * atan(1.0f / zoom); }

#endif 
