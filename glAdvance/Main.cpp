#include <windows.h>
//glew
#include <GL/glew.h>
#include <GL/GL.h>


//glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <SOIL.h>

#include "Camera.h"

#include <limits>
#include <iostream>

//glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//
#include "math/AABB3.h"

int screenWidth = 800;
int screenHeight = 600;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double posx, double posy);
void mouse_button_callback(GLFWwindow* window, int b, int action, int);
void scroll_callback(GLFWwindow* window, double offsetx, double offsety);
void coordinate_system(GLFWwindow* window);
void lighting_system(GLFWwindow* window);
void blending(GLFWwindow* window);
void framebuffer(GLFWwindow* window);
void cubemap(GLFWwindow* window);
void advancedGLSL(GLFWwindow* window);
void blinnPhone(GLFWwindow* window);
void GammaCorrection(GLFWwindow* window);

GLuint loadCubemapTexture(std::vector<const char*>& faces);

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, void *userParam);

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
void RenderSceneFBO(Shader& planShader,
	Shader& screenShader,
	GLuint planVAO,
	GLuint planTexture,
	GLuint cubeVAO,
	GLuint cubeTexture,
	GLuint textureColorbuffer,
	GLuint screenVAO,
	GLuint fbo);
void RenderScene(
	Shader& planShader,
	Shader& screenShader,
	GLuint planVAO,
	GLuint planTexture,
	GLuint cubeVAO,
	GLuint cubeTexture);

void do_movement();
void calcFPS(GLFWwindow* window, GLfloat title);
bool keys[1024];
bool keysPressed[1024];
bool blinn = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFram = 0.0f;
GLfloat lastX = 400;
GLfloat lastY = 300;
GLfloat pitch = 0.0f;
GLfloat yaw = -90.0f;
bool firstMouse = true;
GLfloat fov = 45.0f;
glm::vec4 min = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
glm::vec4 max = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); 
glm::mat4 projection;
glm::mat4 view;

glm::vec3 ray;
bool drawLine = false;

void screenCoordsTo3DCoords(double x, double y);

//----------------------------------------------------------
AABB3 aabb;
//----------------------------------------------------------

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLuint CreateTexture(const char* file, bool alpha = false);

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;	
	}
	glGetError();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//7--8
	//coordinate_system(window);
	//9 color
	//lighting_system(window);
	//10 blend
	blending(window);
	//11 framebuffer
	//framebuffer(window);
	//12 cubemap
	//cubemap(window);
	//13 advanced glsl
	//advancedGLSL(window);

	//blinnPhone(window);
	//GammaCorrection(window);	

	glfwTerminate();
	
	return 0;
}

void GammaCorrection(GLFWwindow* window)
{
	//顶点数据
	//1 面
	GLuint planVAO;
	GLuint planVBO;
	GLfloat quadVertices[] = {
		// Positions        // Texture Coords
		5.0f, -0.5f, 5.0f, 1.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 1.0f,

		5.0f, -0.5f, 5.0f, 1.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 1.0f,
		5.0f, -0.5f, -5.0f, 1.0f, 1.0f
	};
	// Setup plane VAO
	glGenVertexArrays(1, &planVAO);
	glGenBuffers(1, &planVBO);
	glBindVertexArray(planVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//shader 
	Shader shader("gammacorrection/gammacorrect.vs", "gammacorrection/gammacorrect.frag");

	//texture
	GLuint planeTexture = CreateTexture("wood.png");

	while (!glfwWindowShouldClose(window))
	{
		//检查及调用事件
		glfwPollEvents();
		do_movement();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calc fps
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFram;
		lastFram = currentFrame;
		calcFPS(window, lastFram);

		shader.use();
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		glm::mat4 tran;
		

		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth/screenHeight, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(planVAO);
		glBindTexture(GL_TEXTURE_2D,planeTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}


}
void blinnPhone(GLFWwindow* window)
{
	//glEnable(GL_FRAMEBUFFER_SRGB);
	glm::vec3 lightPos = { 0.0f, 0.0f, 0.0f };

	//顶点数据
	//1 面
	GLuint planVAO;
	GLuint planVBO;

	GLfloat planeVertices[] = {
		// Positions          // Normals     // Texture Coords
		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		-8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
		
		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
		8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f
		 };
	// Setup plane VAO
	glGenVertexArrays(1, &planVAO);
	glGenBuffers(1, &planVBO);
	glBindVertexArray(planVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//shader 
	Shader shader("blinn/blinn.vs", "blinn/blinn.frag");

	//texture
	GLuint planeTexture = CreateTexture("wood.png");

	while (!glfwWindowShouldClose(window))
	{
		//检查及调用事件
		glfwPollEvents();
		do_movement();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calc fps
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFram;
		lastFram = currentFrame;
		calcFPS(window, lastFram);

		shader.use();
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		view = camera.GetViewMatrix();
		float* ma = glm::value_ptr(view);

		for (int i = 0; i < 16; ++i)
		{			
			std::cout << ma[i] << std::endl;			
		}
		float mm[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 
						 0.0f, 1.0f, 0.0f, 0.0f, 
						 0.0f, 0.0f, 1.0f, 0.0f, 
						 0.0f, 0.0f, 0.0f, 1.0f, };
		glm::mat4 bb = glm::make_mat4(mm);

		projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth / screenHeight, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//light unform 
		glUniform3fv(glGetUniformLocation(shader.program, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(shader.program, "viewPos"), 1, &camera.Position[0]);
		glUniform1i(glGetUniformLocation(shader.program, "blinn"),  blinn);

		glBindVertexArray(planVAO);
		glBindTexture(GL_TEXTURE_2D, planeTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
}
void advancedGLSL(GLFWwindow* window)
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//vertices
	//2 box
	GLuint cubeVAO;
	GLuint cubeVBO;
	GLfloat vertices[] = {
		// Positions          // Normals
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	glGenBuffers(1, &cubeVBO);
	glGenVertexArrays(1, &cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	//shader
	Shader shader("advancedglsl/advancedglsl.vs", "advancedglsl/advancedglsl.frag");
	shader.use();
	//loop
	while (!glfwWindowShouldClose(window))
	{
		//检查及调用事件
		glfwPollEvents();
		do_movement();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calc fps
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFram;
		lastFram = currentFrame;
		calcFPS(window, lastFram);
		
		shader.use();

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth / screenHeight, 0.1f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
	}
}

void cubemap(GLFWwindow* window)
{
	//2 box
	GLuint cubeVAO;
	GLuint cubeVBO;
	GLfloat vertices[] = {
		// Positions          // Normals
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	glGenBuffers(1, &cubeVBO);
	glGenVertexArrays(1, &cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//skybox
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};
	GLuint skyboxVAO, skyboxVBO;
	glGenBuffers(1, &skyboxVBO);
	glGenVertexArrays(1, &skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//shader
	Shader planShader("cubemap/cube.vs", "cubemap/cube.frag");
	Shader skyboxShader("cubemap/cubemap.vs", "cubemap/cubemap.frag");

	//texture
	//GLuint cubeTexture = CreateTexture("marble.jpg");

	std::vector<const char*> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");

	GLuint cubemapTexture = loadCubemapTexture(faces);

	while (!glfwWindowShouldClose(window))
	{
		//检查及调用事件
		glfwPollEvents();
		do_movement();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calc fps
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFram;
		lastFram = currentFrame;
		calcFPS(window, lastFram);

		skyboxShader.use();
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth / screenHeight, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//skybox
		//glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//glDepthMask(GL_TRUE);
		

		//cube
		planShader.use();
		model = glm::mat4();
		view = glm::mat4();
		view = camera.GetViewMatrix();
		
		glUniform3f(glGetUniformLocation(planShader.program, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
}

GLuint loadCubemapTexture(std::vector<const char*>& faces)
{
	//创建纹理对象
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	//加载纹理
	int iwidth, iheight;
	unsigned char* image;
	for (int i = 0; i < faces.size(); ++i)
	{
		image = SOIL_load_image(faces.at(i), &iwidth, &iheight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	//纹理采样方式
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return texture;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
		keysPressed[key] = false;
	}

}
void mouse_callback(GLFWwindow* window, double posx, double posy)
{
	if (firstMouse)
	{
		lastX = posx;
		lastY = posy;
		firstMouse = false;
	}

	GLfloat offsetx = posx - lastX;
	GLfloat offsety = lastY - posy;
	lastX = posx;
	lastY = posy;

	camera.ProcessMouseMovement(offsetx, offsety);

}
void scroll_callback(GLFWwindow* window, double offsetx, double offsety)
{
	camera.ProcessMouseScroll(offsety);
}
void do_movement()
{

	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
		//GLuint tex_2d = SOIL_save_screenshot("screenshot.bmp", SOIL_SAVE_TYPE_BMP, 0, 0, 800, 600);
	}
	else if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	else if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	else if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_B] && !keysPressed[GLFW_KEY_B])
	{
		blinn = !blinn;
		keysPressed[GLFW_KEY_B] = true;
	}
}
void calcFPS(GLFWwindow* window, GLfloat title)
{
	static std::string temp = "";
	static GLfloat framesPerSecond = 0.0f;
	static GLfloat frames = 0.0f;
	static GLfloat lastTime = 0.0f;
	GLfloat currentTime = glfwGetTime();
	++frames;
	if (currentTime - lastTime > 1.0f)
	{
		framesPerSecond = frames;
		temp = "fps: " + std::to_string(framesPerSecond);
		lastTime = currentTime;
		frames = 0;
	}
	glfwSetWindowTitle(window, temp.c_str());
}
//lighting 
void lighting_system(GLFWwindow* window)
{
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f); 

	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
// 		glm::vec3(2.0f, 5.0f, -15.0f),
// 		glm::vec3(-1.5f, -2.2f, -2.5f),
// 		glm::vec3(-3.8f, -2.0f, -12.3f),
// 		glm::vec3(2.4f, -0.4f, -3.5f),
// 		glm::vec3(-1.7f, 3.0f, -7.5f),
// 		glm::vec3(1.3f, -2.0f, -2.5f),
// 		glm::vec3(1.5f, 2.0f, -2.5f),
// 		glm::vec3(1.5f, 0.2f, -1.5f),
// 		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	//lamp
	GLuint lampVBO;
	glGenBuffers(1, &lampVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);

	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lampVAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 8, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//object
	GLuint objectVBO;
	glGenBuffers(1, &objectVBO);
	glBindBuffer(GL_ARRAY_BUFFER, objectVBO);

	GLuint objectVAO;
	glGenVertexArrays(1, &objectVAO);
	glBindVertexArray(objectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, objectVAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 8, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 8, (GLvoid*)(sizeof(GLfloat)* 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)* 8, (GLvoid*)(sizeof(GLfloat)* 6));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	GLuint diffuseMap = CreateTexture("container2.png");
	GLuint specularMap = CreateTexture("container2_specular.png");

	Shader lampShader("lamp.vs", "lamp.frag");
	Shader objectShader("object.vs", "object.frag");
	Shader signalColorShader("object.vs", "signalcolor.frag");

	objectShader.use();
	glUniform1i(glGetUniformLocation(objectShader.program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(objectShader.program, "material.specular"), 1);

	while (!glfwWindowShouldClose(window))
	{
		//监听事件
		glfwPollEvents();
		do_movement();

		GLfloat currentFram = glfwGetTime();
		deltaTime = currentFram - lastFram;
		lastFram = currentFram;
		calcFPS(window, lastFram);

		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glStencilMask(0x00);		
		
		//draw lamp
		lampShader.use();
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)screenWidth / screenHeight, 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(lampShader.program, "model");
		GLint viewLoc = glGetUniformLocation(lampShader.program, "view");
		GLint projLoc = glGetUniformLocation(lampShader.program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glm::mat4 model;
// 		lightPos.x = sin(glfwGetTime());
// 		lightPos.z = cos(glfwGetTime());
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		glStencilFunc(GL_ALWAYS, 1, 0xFF); 
		glStencilMask(0xFF);
	
		//draw object
		objectShader.use();
	
		GLint lightPosLoc = glGetUniformLocation(objectShader.program, "lightPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		GLint viewPosLoc = glGetUniformLocation(objectShader.program, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		GLint shininessLoc = glGetUniformLocation(objectShader.program, "material.shininess");
		glUniform1f(shininessLoc, 32.0f);

		GLint lightambientLoc = glGetUniformLocation(objectShader.program, "light.ambient");
		GLint lightdiffuseLoc = glGetUniformLocation(objectShader.program, "light.diffuse");
		GLint lightspecularLoc = glGetUniformLocation(objectShader.program, "light.specular");

		glUniform3f(lightambientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightdiffuseLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightspecularLoc, 1.0f, 1.0f, 1.0f);

		modelLoc = glGetUniformLocation(objectShader.program, "model");
		viewLoc = glGetUniformLocation(objectShader.program, "view");
		projLoc = glGetUniformLocation(objectShader.program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLint lightDirPos = glGetUniformLocation(objectShader.program, "light.direction");
		glUniform3f(lightDirPos, -0.2f, -1.0f, -0.3f);		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		model = glm::mat4();
		model = glm::translate(model, cubePositions[0]);
		model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			
		glm::vec4 worldMin = model * min;
		glm::vec4 worldMax = model * max;
		aabb.min = Vector3(worldMin.x, worldMin.y, worldMin.z);
		aabb.max = Vector3(worldMax.x, worldMax.y, worldMax.z);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(objectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);		
		glBindVertexArray(0);

		if (drawLine)
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			signalColorShader.use();

			modelLoc = glGetUniformLocation(signalColorShader.program, "model");
			viewLoc = glGetUniformLocation(signalColorShader.program, "view");
			projLoc = glGetUniformLocation(signalColorShader.program, "projection");

			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			model = glm::mat4();
			model = glm::translate(model, cubePositions[0]);
			model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(objectVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);
		}		
		
		//交换缓冲
		glfwSwapBuffers(window);
	}
}
//7, 坐标系统
void coordinate_system(GLFWwindow* window)
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
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


	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

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


	while (!glfwWindowShouldClose(window))
	{
		//监听事件
		glfwPollEvents();
		do_movement();

		GLfloat currentFram = glfwGetTime();
		deltaTime = currentFram - lastFram;
		lastFram = currentFram;
		calcFPS(window, lastFram);

		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		GLint modelLoc = glGetUniformLocation(shader.program, "model");

		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		
		GLint viewLoc = glGetUniformLocation(shader.program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//projection
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)screenWidth / screenHeight, 0.1f, 100.0f);
		GLint projectionLoc = glGetUniformLocation(shader.program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20 * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		//交换缓冲

	}
}

GLuint CreateTexture(const char* file, bool alpha)
{
	//创建纹理对象
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//纹理采样方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//加载纹理
	int iwidth, iheight;
	unsigned char* image = SOIL_load_image(file, &iwidth, &iheight, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_SRGB, iwidth, iheight, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}
void screenCoordsTo3DCoords(double posx, double posy)
{
// 	GLint  viewport[4];
// 	GLdouble modelview[16] = { 0.0f };
// 	GLdouble projectionMat[16] = { 0.0f };
// 	GLfloat winx = posx;
// 	GLfloat winy = screenHeight - posy;
// 	GLfloat winz;
// 	GLdouble wx, wy, wz;
// 
// 	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
// 	glGetDoublev(GL_PROJECTION_MATRIX, projectionMat);
// 	glGetIntegerv(GL_VIEWPORT, viewport);
// 	glReadPixels(winx, winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
// 	gluUnProject(winx, winy, winz, modelview, projectionMat, viewport, &wx, &wy, &wz);

	float x = (2.0f * posx) / screenWidth - 1.0;
	float y = 1.0 - (2.0f * posy) / screenHeight;
	float z = 1.0;// far near -1.0;
	
	glm::vec3 ray_ndc = glm::vec3(x, y, z);

	glm::vec4 ray_clip = glm::vec4(ray_ndc.x, ray_ndc.y, ray_ndc.z, 1.0);
	glm::vec4 ray_view = glm::inverse(projection) * ray_clip;
	glm::vec4 ray_world = glm::inverse(view) * ray_view;

	if (ray_world.w != 0.0f)
	{
		ray_world.x /= ray_world.w;
		ray_world.y /= ray_world.w;
		ray_world.z /= ray_world.w;
	}

	glm::vec3 tmpworld = glm::vec3(ray_world.x, ray_world.y, ray_world.z);
	ray = glm::normalize( tmpworld - camera.Position);

	Vector3 origin(0.0f, 0.0f, 3.0f);
	Vector3 direction(tmpworld.x, tmpworld.y, tmpworld.z);
	Vector3 returnNormal;

	float rets = aabb.RayIntersect(origin, direction, &returnNormal);
	
	std::cout << rets << std::endl;
	if (rets > 1.0f)
	{
		std::cout << "不相交" << std::endl;
		drawLine = false;
	}
	else
	{
		drawLine = true;
		std::cout << "相交" << std::endl;
	}

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
// 	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
// 	{
// 		std::cout << "clicked" << std::endl;
// 		screenCoordsTo3DCoords(lastX, lastY);
// 	}
}

void blending(GLFWwindow* window)
{
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//顶点数据
	//1 面
	GLuint planVAO;
	GLuint planVBO;
	GLfloat quadVertices[] = {
		// Positions        // Texture Coords
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};
	// Setup plane VAO
	glGenVertexArrays(1, &planVAO);
	glGenBuffers(1, &planVBO);
	glBindVertexArray(planVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//2 box
	GLuint cubeVAO;
	GLuint cubeVBO;
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// Fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Link vertex attributes
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//
	GLfloat transparentVertices[] = {
		// Positions        
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 0.0f, 1.0f, 0.0f
	};
	GLuint tranVAO;
	GLuint tranVBO;
	glGenVertexArrays(1, &tranVAO);
	glGenBuffers(1, &tranVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tranVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

	glBindVertexArray(tranVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//shader
	Shader planShader("blending/blending.vs", "blending/blending.frag");
	//Shader cubeShader("blending/blending.vs", "blending/blending.frag");

	//纹理
	GLuint planTexture = CreateTexture("floor.png", true);
	GLuint cubeTexture = CreateTexture("Block5.png", true);
	GLuint transTexture = CreateTexture("blending_transparent_window.png", true);

	planShader.use();

	std::vector<glm::vec3> windows;
	windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

// 	//fbo
// 	GLuint fbo;
// 	glGenFramebuffers(1, &fbo);
// 	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
// 
// 	//rbo
// 	GLuint rbo;
// 	glGenRenderbuffers(1, &rbo);
// 	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
// 	//创建深度和模板缓冲对象
// 	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
// 	//为渲染缓冲分配内存后解绑渲染缓冲
// 	glBindRenderbuffer(GL_RENDERBUFFER, 0);
// 
// 	//将渲染缓冲对象附加到帧缓冲深度和模板附件上
// 	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_FRAMEBUFFER, rbo);
// 	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
// 	{
// 		std::cout << "Framebuffer is not complete!" << std::endl;
// 	}
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//循环
	while (!glfwWindowShouldClose(window))
	{
		//检查及调用事件
		glfwPollEvents();
		do_movement();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFram;
		lastFram = currentFrame;
		calcFPS(window, lastFram);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		planShader.use();
		glm::mat4 model;
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
		
		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));		
		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		

		glDisable(GL_CULL_FACE);
		//cube0
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));

		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//启用
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendColor(0, 0, 0, 0.4f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//floor
		model = glm::mat4();
		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, planTexture);
		glBindVertexArray(planVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		//关闭
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		//cube1
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


// 		//window
// 		glBindTexture(GL_TEXTURE_2D, transTexture);
// 		glBindVertexArray(tranVAO);
// 		for (auto &x : windows)
// 		{
// 			model = glm::mat4();
// 			model = glm::translate(model, x);
// 			glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
// 			glDrawArrays(GL_TRIANGLES, 0, 6);
// 		}
// 		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}
void framebuffer(GLFWwindow* window)
{
	GLfloat screenVertices[] = {
		// Positions   // TexCoords
		-0.3f, 1.0f, 0.0f, 1.0f,
		-0.3f, 0.7f, 0.0f, 0.0f,
		0.3f, 0.7f, 1.0f, 0.0f,

		-0.3f, 1.0f, 0.0f, 1.0f,
		0.3f, 0.7f, 1.0f, 0.0f,
		0.3f, 1.0f, 1.0f, 1.0f
	};

	GLuint screenVAO, screenVBO;
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//顶点数据
	//1 面
	GLuint planVAO;
	GLuint planVBO;
	GLfloat quadVertices[] = {
		// Positions        // Texture Coords
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};
	// Setup plane VAO
	glGenVertexArrays(1, &planVAO);
	glGenBuffers(1, &planVBO);
	glBindVertexArray(planVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//2 box
	GLuint cubeVAO;
	GLuint cubeVBO;
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// Fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Link vertex attributes
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//shader
	Shader planShader("blending/blending.vs", "blending/blending.frag");
	Shader screenShader("framebuffer/framebuffer.vs", "framebuffer/framebuffer.frag");

	//纹理
	GLuint planTexture = CreateTexture("metal.png");
	GLuint cubeTexture = CreateTexture("marble.jpg");
	GLuint transTexture = CreateTexture("blending_transparent_window.png", true);

	//fbo
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//color buffer texture
	GLuint textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//rbo
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//创建深度和模板缓冲对象
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	//为渲染缓冲分配内存后解绑渲染缓冲
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//将渲染缓冲对象附加到帧缓冲深度和模板附件上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//循环
	while (!glfwWindowShouldClose(window))
	{
		//检查及调用事件
		glfwPollEvents();
		do_movement();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFram;
		lastFram = currentFrame;
		calcFPS(window, lastFram);
		
		RenderScene(planShader, screenShader, planVAO, planTexture, cubeVAO, cubeTexture);
		RenderSceneFBO(planShader, screenShader, planVAO, planTexture, cubeVAO,cubeTexture,textureColorbuffer, screenVAO, fbo);	
		
		glfwSwapBuffers(window);
	}

	// Clean up
	//glDeleteFramebuffers(1, &fbo);

}
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screenWidth, screenHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;

}

void RenderScene(Shader& planShader, Shader& screenShader, GLuint planVAO, GLuint planTexture, GLuint cubeVAO, GLuint cubeTexture)
{
	glEnable(GL_DEPTH_TEST);
	planShader.use();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model;
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// Floor
	glBindVertexArray(planVAO);
	glBindTexture(GL_TEXTURE_2D, planTexture);
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	// Cubes
	glBindVertexArray(cubeVAO);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);	

}

void RenderSceneFBO(Shader& planShader,
					Shader& screenShader,
					GLuint planVAO,
					GLuint planTexture,
					GLuint cubeVAO,
					GLuint cubeTexture,

					GLuint textureColorbuffer,
					GLuint screenVAO,
					GLuint fbo)
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	planShader.use();
	camera.Yaw += 180;
	camera.Pitch += 180;

	camera.ProcessMouseMovement(0, 0, false);
	glm::mat4 view = camera.GetViewMatrix();
	camera.Yaw -= 180;
	camera.Pitch -= 180;
	camera.ProcessMouseMovement(0, 0);
	
	glm::mat4 model;
	
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Floor
	glBindVertexArray(planVAO);
	glBindTexture(GL_TEXTURE_2D, planTexture);
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	// Cubes
	glBindVertexArray(cubeVAO);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(planShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Draw Screen
	glDisable(GL_DEPTH_TEST);
	screenShader.use();
	glBindVertexArray(screenVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	void *userParam)
{

	// 忽略一些不重要的错误/警告代码
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

