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
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // ����
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // ����
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // ����
	};

	GLint indices[] =
	{
		0, 1, 3,
		1, 2, 3
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//���ö�������λ��
	glEnableVertexAttribArray(2);
	//���VAO
	glBindVertexArray(0);

	Shader shader("shader.vs", "shader.frag");


	//�����������
	GLuint texture0, texture1;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	
	//���������ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//��������
	int iwidth, iheight;
	unsigned char* image = SOIL_load_image("container.jpg", &iwidth, &iheight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//���������ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//����������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	image = SOIL_load_image("cawesomeface.png", &iwidth, &iheight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//������ɫ��
// 	const char* vertexShaderSource = "#version 330 core\n"
// 								"layout(location=0) in vec3 position; \n"
// 								"layout(location=1) in vec3 color; \n"
// 								"out vec3 myColor; \n"
// 								"void main()\n"
// 								"{\n"
// 								"gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
// 								"myColor = color; \n"
// 								"}\n\0";
// 	//������ɫ��
// 	GLuint vertexShader;
// 	vertexShader = glCreateShader(GL_VERTEX_SHADER);
// 	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
// 	glCompileShader(vertexShader);
// 
// 	//�ж��Ƿ����ɹ�
// 	GLint success;
// 	GLchar infoLog[512];
// 	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
// 	if (!success)
// 	{
// 		//���ʧ����Ϣ
// 		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
// 		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
// 	}
// 	//Ƭ����ɫ��
// 	const GLchar* fragmentShaderSource = "#version 330 core\n"
// 		"in vec3 myColor; \n"
// 		"out vec4 color;\n"
// 		"void main()\n"
// 		"{\n"
// 		"color = vec4(myColor, 1.0f);\n"
// 		"}\n\0";
// 
// 	//����Ƭ����ɫ��
// 	GLuint 	fragmentShader;
// 	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
// 	//����ɫ��Դ�븽�ӵ���ɫ��������
// 	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
// 	//������ɫ��
// 	glCompileShader(fragmentShader);
// 
// 	//����һ����ɫ���������
// 	GLuint shaderProgram;
// 	shaderProgram = glCreateProgram();
// 	//���������ɫ�����ӵĶ�����
// 	glAttachShader(shaderProgram, vertexShader);
// 	glAttachShader(shaderProgram, fragmentShader);
// 	//������ɫ��
// 	glLinkProgram(shaderProgram);
// 
// 	//�����Ƿ����ӳɹ�
// 	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
// 	if (!success)
// 	{
// 		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
// 		std::cout << infoLog << std::endl;
// 	}
// 
// 	//����������
// 	glUseProgram(shaderProgram);
// 
// 	//���Ӻ�ɾ����ɫ������
// 	glDeleteShader(vertexShader);
// 	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		//�����¼�
		glfwPollEvents();
		//��Ⱦָ��
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