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

	//��������
	GLfloat vertexs[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f

	};
	//�������㻺�����
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//������󶨵�GL_ARRAY_BUFFER Ŀ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//���������������
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	//��VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);

	//�Ѷ������ݿ�������ǰ�󶨵Ļ����ϣ����㻺�壩
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//���ö�������λ��
	glEnableVertexAttribArray(0);

	//���VAO
	glBindVertexArray(0);

	//������ɫ��
	const char* vertexShaderSource = "#version 330 core\n"
									"layout(location=0) in vec3 position;\n"
									"void main()\n"
									"{\n"
									"gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
									"}\n\0";
	//������ɫ��
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//�ж��Ƿ����ɹ�
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//���ʧ����Ϣ
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//Ƭ����ɫ��
	const GLchar* fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	//����Ƭ����ɫ��
	GLuint 	fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//����ɫ��Դ�븽�ӵ���ɫ��������
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//������ɫ��
	glCompileShader(fragmentShader);

	//����һ����ɫ���������
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	//���������ɫ�����ӵĶ�����
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//������ɫ��
	glLinkProgram(shaderProgram);

	//�����Ƿ����ӳɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	//����������
	glUseProgram(shaderProgram);

	//���Ӻ�ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		//�����¼�
		glfwPollEvents();
		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		//��������
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