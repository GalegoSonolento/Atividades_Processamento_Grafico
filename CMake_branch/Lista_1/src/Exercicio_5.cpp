#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include "glad.h"
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 400\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupShader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupGeometry()
{
	GLfloat vertices[] = {
		//x     y    z
		//T0
		-0.99,  0.0, 0.0, //v0
		-0.2,   0.5, 0.0, //v1
 		-0.2,  -0.5, 0.0, //v2
		//T1
        //x     y    z
        0.99,  0.0, 0.0, //v3
        0.2,   0.5, 0.0, //v4
        0.2,  -0.5, 0.0, //v5
        //T2
        //x    y      z
        -0.99, 0.2,  0.0, //v6
        -0.2,  0.7,  0.0, //v7
        -0.99, 0.99, 0.0, //v8
        //T3
        //x    y     z
        0.99,  0.2,  0.0, //v9
        0.2,  0.7,  0.0, //v10
        0.99, 0.99, 0.0, //v11
		//T4
        //x     y      z
        -0.99, -0.2,  0.0, //v12
        -0.99, -0.99, 0.0, //v13
        -0.2,  -0.7,  0.0, //v14
		//T5
        //x     y     z
        0.99, -0.2,  0.0, //v15
        0.99, -0.99, 0.0, //v16
        0.2,  -0.7,  0.0  //v17
			  
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	return VAO;
}

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Henrique", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER); 
	const GLubyte* version = glGetString(GL_VERSION); 
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLuint shaderID = setupShader();

	GLuint VAO = setupGeometry();

	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	
	glUseProgram(shaderID);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.95f, 0.70f, 0.10f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glBindVertexArray(VAO);

		glUniform4f(colorLoc, 0.07f, 0.23f, 0.34f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUniform4f(colorLoc, 0.07f, 0.23f, 0.34f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 6, 3);
        glDrawArrays(GL_LINE_LOOP, 9, 3);

		glUniform4f(colorLoc, 0.07f, 0.23f, 0.34f, 1.0f);
		glDrawArrays(GL_POINTS, 12, 3);
		glDrawArrays(GL_POINTS, 15, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

