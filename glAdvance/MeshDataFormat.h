#ifndef  _MESH_DATA_FORMAT_H_
#define _MESH_DATA_FORMAT_H_
//----------------------------------------------------------------------------------
/**
\brief 顶点属性类型
*/
enum AttribType
{
	atUINT8,   /**< 8位整数格式 */
	atUINT16,  /**< 16位整数格式 */
	atUINT32,  /**< 32位整数格式 */
	atFLOAT32, /**< 32位浮点格式 */
};
//----------------------------------------------------------------------------------
/**
\brief 顶点存储模式
*/
enum DateStorage
{
	dsSeparate,   /**< 各分量分离存储 */
	dsInterlaced, /**< 各分量交错存储 */
};
//----------------------------------------------------------------------------------
/**
\brief 数据分量说明
*/
struct aeDataComponentDesc
{
public:
	/**
	\brief 分量数据数量
	*/
	unsigned long Count;
	/**
	\brief 分量数据格式
	*/
	AttribType Type;
};
//----------------------------------------------------------------------------------
/**
\brief 数据格式说明
*/
struct DataFormatDesc
{
	enum
	{
		/**
		\brief 数据分量数量
		*/
		ComponentCount = 17U,
	};
	/**
	数据存储方式
	*/
	DateStorage Storage;
	/**
	数据单元长度(单位字节)
	*/
	unsigned long Length;
	/**
	分量说明
	*/
	aeDataComponentDesc Desc[ComponentCount];
};
//----------------------------------------------------------------------------------
/**
\brief 数据信息
*/
struct DataInfo
{
	/**
	\brief 顶点格式说明
	*/
	const DataFormatDesc* Format;
	/**
	\brief 顶点数量
	*/
	int Count;
	/**
	\brief 顶点数据
	*/
	const void* Data;
};
//----------------------------------------------------------------------------------
#endif