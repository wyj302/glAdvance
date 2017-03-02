#include <windows.h>
//glew
#define GLEW_STATIC
#include <GL/glew.h>

//glfw
#include <GLFW/glfw3.h>
#include <iostream>

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

	//顶点数组
	GLfloat vertexs[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//启用顶点属性位置
	glEnableVertexAttribArray(0);

	//解绑VAO
	glBindVertexArray(0);

	//顶点着色器
	const char* vertexShaderSource = "#version 330 core\n"
									"layout(location=0) in vec3 position;\n"
									"void main()\n"
									"{\n"
									"gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
									"}\n\0";
	//编译着色器
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//判断是否编译成功
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//获得失败信息
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//片段着色器
	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	//编译片段着色器
	GLuint 	fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//将着色器源码附加到着色器对象上
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//编译着色器
	glCompileShader(fragmentShader);

	//创建一个着色器程序对象
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	//将编译的着色器附加的对象上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//链接着色器
	glLinkProgram(shaderProgram);

	//检验是否链接成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	//激活程序对象
	glUseProgram(shaderProgram);

	//链接后删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		//监听事件
		glfwPollEvents();
		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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