#ifndef _MESH_H_
#define _MESH_H_
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include "MeshDataFormat.h"

/**
\brief �����ж�ȡ����
\param in ������
\param data Ҫ��ȡ������
\return �Ƿ��ȡ�ɹ�
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