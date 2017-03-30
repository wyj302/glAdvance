//================================================
// 4*4����ʵ����תƽ�Ƶȴ󲿷ֹ��ܡ�
// ���ա�3D��ѧ��������RotationMatrix��Matrix4x3��
// ��������OpenGL���
// Copy Rights: Wonderful 2006
//================================================

#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

//-------------------------------------------------------------------------------
// ǰ������
//-------------------------------------------------------------------------------
class Vector3;

//-------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------
class CMatrix44F
{
public:
	// ���õ�λ����
	void Identity();	

	// �����к��С�
	void GetRow(int i, Vector3 *pVec) const;	//����GetRow(3, &position);���Եõ�λ�á�
	void GetCol(int i, Vector3 *pVec) const;
	void SetRow(int i, const Vector3 &vec);
	void SetCol(int i, const Vector3 &vec);


	//---------------------------ƽ�Ʋ���----------------------------------
	// �ƶ������ÿ�
	void ZeroTranslation();						
	// ֱ�������ƶ�����
	void SetTranslation(const Vector3 &d);		
	// �����ƶ����ֲ�����ת�ÿ�
	void SetupTranslation(const Vector3 &d);	
	// ���ݵ�ǰ����ת��λ�����ƶ�����
	void Translate(const Vector3 &d);			

	//---------------------------��ת����----------------------------------
	// ������������1��x�ᣬ2��y�ᣬ3��z�ᡣ

	// ��������������ת�ľ���
	void SetupRotate(int axis, float theta);			
	// ��������������ת�ľ���
	void SetupRotate(const Vector3 &axis, float theta);	
	// ����������ת�������յ�ǰ�ľ���
	void Rotate(int axis, float theta);					
	// ����������ת�������յ�ǰ�ľ���
	void Rotate(const Vector3 &axis, float theta);		

	// ������ת�ĳ˷���ƽ�Ʋ��ֲ��䡣34Ϊ�������е���˼
	CMatrix44F& Mul34(const CMatrix44F &m);
	// ���䷴ת�����ڡ����嵽���硱�͡����絽���塱��ת��
	CMatrix44F& AffineInverse(); 

public:
	float m[16];				// �������ݳ�Ա
};

//-------------------------------------------------------------------------------
// �ǳ�Ա����
//-------------------------------------------------------------------------------
// �������Ծ���
Vector3    operator*(const Vector3 &v, const CMatrix44F &m);
// ������ˣ�����ÿ������캯������һ��
CMatrix44F operator*(const CMatrix44F &a, const CMatrix44F &b);
// �������Ծ���
Vector3&    operator*=(Vector3 &v, const CMatrix44F &m);
// ������Ծ���
CMatrix44F& operator*=(CMatrix44F &a, const CMatrix44F &b);

// ����3X3���ֵ�����ʽֵ
float Determinant(const CMatrix44F &m);

// ����������
CMatrix44F Inverse(const CMatrix44F &m);

// ��ȡ�����ƽ�Ʋ���
Vector3	GetTranslation(const CMatrix44F &m);

// �Ӿֲ�����->�����󣬻򸸾���->�ֲ�������ȡλ��
Vector3	GetPositionFromParentToLocalMatrix(const CMatrix44F &m);
Vector3	GetPositionFromLocalToParentMatrix(const CMatrix44F &m);

#endif 