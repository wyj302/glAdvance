#include <windows.h>
//glew
#define GLEW_STATIC
#include <GL/glew.h>

//glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <SOIL.h>

//glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // 左上
	};

	GLint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	//创建顶点缓冲对象
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//将缓冲绑定到GL_ARRAY_BUFFER 目标上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//创建顶点数组对象
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	//绑定VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);

	//把顶点数据拷贝到当前绑定的缓冲上（顶点缓冲）
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//启用顶点属性位置
	glEnableVertexAttribArray(2);
	//解绑VAO
	glBindVertexArray(0);

	Shader shader("shader.vs", "shader.frag");


	//创建纹理对象
	GLuint texture0, texture1;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	
	//纹理采样方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//加载纹理
	int iwidth, iheight;
	unsigned char* image = SOIL_load_image("container.jpg", &iwidth, &iheight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//纹理采样方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image = SOIL_load_image("cawesomeface.png", &iwidth, &iheight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//顶点着色器
// 	const char* vertexShaderSource = "#version 330 core\n"
// 								"layout(location=0) in vec3 position; \n"
// 								"layout(location=1) in vec3 color; \n"
// 								"out vec3 myColor; \n"
// 								"void main()\n"
// 								"{\n"
// 								"gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
// 								"myColor = color; \n"
// 								"}\n\0";
// 	//编译着色器
// 	GLuint vertexShader;
// 	vertexShader = glCreateShader(GL_VERTEX_SHADER);
// 	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
// 	glCompileShader(vertexShader);
// 
// 	//判断是否编译成功
// 	GLint success;
// 	GLchar infoLog[512];
// 	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
// 	if (!success)
// 	{
// 		//获得失败信息
// 		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
// 		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
// 	}
// 	//片段着色器
// 	const GLchar* fragmentShaderSource = "#version 330 core\n"
// 		"in vec3 myColor; \n"
// 		"out vec4 color;\n"
// 		"void main()\n"
// 		"{\n"
// 		"color = vec4(myColor, 1.0f);\n"
// 		"}\n\0";
// 
// 	//编译片段着色器
// 	GLuint 	fragmentShader;
// 	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
// 	//将着色器源码附加到着色器对象上
// 	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
// 	//编译着色器
// 	glCompileShader(fragmentShader);
// 
// 	//创建一个着色器程序对象
// 	GLuint shaderProgram;
// 	shaderProgram = glCreateProgram();
// 	//将编译的着色器附加的对象上
// 	glAttachShader(shaderProgram, vertexShader);
// 	glAttachShader(shaderProgram, fragmentShader);
// 	//链接着色器
// 	glLinkProgram(shaderProgram);
// 
// 	//检验是否链接成功
// 	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
// 	if (!success)
// 	{
// 		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
// 		std::cout << infoLog << std::endl;
// 	}
// 
// 	//激活程序对象
// 	glUseProgram(shaderProgram);
// 
// 	//链接后删除着色器对象
// 	glDeleteShader(vertexShader);
// 	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		//监听事件
		glfwPollEvents();
		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

// 		GLfloat timeValue = glfwGetTime();
// 		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
// 		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glUniform1i(glGetUniformLocation(shader.program, "texture0"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader.program, "texture1"), 1);

		shader.use();
		GLint transform = glGetUniformLocation(shader.program, "transform");
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians( (GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		/*trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));*/
		glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans))
			;
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//second
		trans = glm::mat4();
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		GLfloat scaleA = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scaleA, scaleA, scaleA));
		glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		//交换缓冲
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}