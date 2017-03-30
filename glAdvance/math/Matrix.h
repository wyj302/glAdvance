//================================================
// 4*4矩阵，实现旋转平移等大部分功能。
// 参照《3D数学基础》的RotationMatrix和Matrix4x3，
// 并方便与OpenGL结合
// Copy Rights: Wonderful 2006
//================================================

#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

//-------------------------------------------------------------------------------
// 前向声明
//-------------------------------------------------------------------------------
class Vector3;

//-------------------------------------------------------------------------------
// 矩阵类
//-------------------------------------------------------------------------------
class CMatrix44F
{
public:
	// 设置单位矩阵
	void Identity();	

	// 访问行和列。
	void GetRow(int i, Vector3 *pVec) const;	//比如GetRow(3, &position);可以得到位置。
	void GetCol(int i, Vector3 *pVec) const;
	void SetRow(int i, const Vector3 &vec);
	void SetCol(int i, const Vector3 &vec);


	//---------------------------平移部分----------------------------------
	// 移动部分置空
	void ZeroTranslation();						
	// 直接设置移动部分
	void SetTranslation(const Vector3 &d);		
	// 设置移动部分并把旋转置空
	void SetupTranslation(const Vector3 &d);	
	// 根据当前的旋转和位置来移动矩阵
	void Translate(const Vector3 &d);			

	//---------------------------旋转部分----------------------------------
	// 坐标轴索引：1、x轴，2、y轴，3、z轴。

	// 构造绕坐标轴旋转的矩阵
	void SetupRotate(int axis, float theta);			
	// 构造绕任意轴旋转的矩阵
	void SetupRotate(const Vector3 &axis, float theta);	
	// 绕坐标轴旋转矩阵，依照当前的矩阵
	void Rotate(int axis, float theta);					
	// 绕任意轴旋转矩阵，依照当前的矩阵
	void Rotate(const Vector3 &axis, float theta);		

	// 用于旋转的乘法，平移部分不变。34为三行四列的意思
	CMatrix44F& Mul34(const CMatrix44F &m);
	// 仿射反转，用于“物体到世界”和“世界到物体”的转变
	CMatrix44F& AffineInverse(); 

public:
	float m[16];				// 矩阵数据成员
};

//-------------------------------------------------------------------------------
// 非成员函数
//-------------------------------------------------------------------------------
// 向量乘以矩阵
Vector3    operator*(const Vector3 &v, const CMatrix44F &m);
// 矩阵相乘，会调用拷贝构造函数，慢一点
CMatrix44F operator*(const CMatrix44F &a, const CMatrix44F &b);
// 向量乘以矩阵
Vector3&    operator*=(Vector3 &v, const CMatrix44F &m);
// 矩阵乘以矩阵
CMatrix44F& operator*=(CMatrix44F &a, const CMatrix44F &b);

// 计算3X3部分的行列式值
float Determinant(const CMatrix44F &m);

// 计算矩阵的逆
CMatrix44F Inverse(const CMatrix44F &m);

// 提取矩阵的平移部分
Vector3	GetTranslation(const CMatrix44F &m);

// 从局部矩阵->父矩阵，或父矩阵->局部矩阵提取位置
Vector3	GetPositionFromParentToLocalMatrix(const CMatrix44F &m);
Vector3	GetPositionFromLocalToParentMatrix(const CMatrix44F &m);

#endif 