#ifndef _MESH_H_
#define _MESH_H_
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include "MeshDataFormat.h"

/**
\brief 从流中读取数据
\param in 输入流
\param data 要读取的数据
\return 是否读取成功
*/
template<typename _T> inline bool Read(std::istream& in, _T& data)
{
	in.read((char*)&data, sizeof(_T));
	return sizeof(_T) == in.gcount();
}

class Mesh
{

public:
	Mesh(const char* file)
	{
		LoadMeshFile(file);
	}
	~Mesh(){}
	GLfloat* GetVertices()
	{
	
	}
	unsigned int* GetIndexs()
	{

	}
	unsigned long GetVerticesCount()
	{
		return vertexCount;
	}
	unsigned long GetIndexCount()
	{
		return indexCount;
	}
private:
	void LoadMeshFile(const char* file)
	{
		std::ifstream in;
		in.open(file, std::ifstream::binary);
		if (!in)
		{
			std::cout << "open file failed." << std::endl;
			return;
		}

	}
private:
	unsigned long  indexCount;
	unsigned long vertexCount;
};
#endif //