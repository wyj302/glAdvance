#ifndef _SHDER_H_
#define  _SHDER_H_
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>;

class Shader
{
public:
	GLuint program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();
};
#endif // !_SHDER_H_
