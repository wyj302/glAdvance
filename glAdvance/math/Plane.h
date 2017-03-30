//////////////////////////////////////////////////////////////////////////
//  CPlane�࣬ƽ���࣬��ײ�Ϳ����Լ���á�
//
//  Plane.h: CPlane�������
//
//  Copy Rights Wonderful 2006
//////////////////////////////////////////////////////////////////////////

#ifndef __PLANE_H_INCLUDED__
#define __PLANE_H_INCLUDED__

//-------------------------------------------------------------------------------
// ǰ������
//-------------------------------------------------------------------------------
class Vector3;
class CMatrix44F;

//-------------------------------------------------------------------------------
// ƽ���࣬��ײ�Ϳ����Լ����
//-------------------------------------------------------------------------------
class CPlane
{
public:
	// ���캯��
	CPlane() {}
	CPlane(const float *pf);
	CPlane(float fa, float fb, float fc, float fd);

	// ǿ��ת������
	operator float *();
	operator const float *() const;

	// һԪ������
	CPlane operator + () const;
	CPlane operator - () const;

	// ����ж�
	bool operator ==(const CPlane &rhs) const;
	bool operator !=(const CPlane &rhs) const;


public:
	float a, b, c, d;		// a��b��cΪ���ߵķ�����dΪ��ԭ�������з��ž���
};

// ƽ�����������㣩���жϵ���ƽ����ı� 
float PlaneDotCoord(const CPlane *pP, const Vector3 *pV);

// �ӵ�ͷ��߹���һ��ƽ�����
CPlane *PlaneFromPointNormal(CPlane *pOut, const Vector3 *pPoint, const Vector3 *pNormal);

// ����ƽ������֪��3������ȡƽ��
CPlane *PlaneFromPoints(CPlane *pOut, const Vector3 *pV1, const Vector3 *pV2, const Vector3 *pV3);

// ƽ��Ĺ�� 
CPlane *PlaneNormalize(CPlane *pOut, const CPlane *pP);

// ƽ��ı任 
CPlane *PlaneTransform(CPlane *pOut, const CPlane *pP, const CMatrix44F *pM);


#endif 
