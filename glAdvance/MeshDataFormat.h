#ifndef  _MESH_DATA_FORMAT_H_
#define _MESH_DATA_FORMAT_H_
//----------------------------------------------------------------------------------
/**
\brief ������������
*/
enum AttribType
{
	atUINT8,   /**< 8λ������ʽ */
	atUINT16,  /**< 16λ������ʽ */
	atUINT32,  /**< 32λ������ʽ */
	atFLOAT32, /**< 32λ�����ʽ */
};
//----------------------------------------------------------------------------------
/**
\brief ����洢ģʽ
*/
enum DateStorage
{
	dsSeparate,   /**< ����������洢 */
	dsInterlaced, /**< ����������洢 */
};
//----------------------------------------------------------------------------------
/**
\brief ���ݷ���˵��
*/
struct aeDataComponentDesc
{
public:
	/**
	\brief ������������
	*/
	unsigned long Count;
	/**
	\brief �������ݸ�ʽ
	*/
	AttribType Type;
};
//----------------------------------------------------------------------------------
/**
\brief ���ݸ�ʽ˵��
*/
struct DataFormatDesc
{
	enum
	{
		/**
		\brief ���ݷ�������
		*/
		ComponentCount = 17U,
	};
	/**
	���ݴ洢��ʽ
	*/
	DateStorage Storage;
	/**
	���ݵ�Ԫ����(��λ�ֽ�)
	*/
	unsigned long Length;
	/**
	����˵��
	*/
	aeDataComponentDesc Desc[ComponentCount];
};
//----------------------------------------------------------------------------------
/**
\brief ������Ϣ
*/
struct DataInfo
{
	/**
	\brief �����ʽ˵��
	*/
	const DataFormatDesc* Format;
	/**
	\brief ��������
	*/
	int Count;
	/**
	\brief ��������
	*/
	const void* Data;
};
//----------------------------------------------------------------------------------
#endif