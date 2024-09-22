#include <iostream>
#include <string>
#include <assert.h>
//biblioteca da linguagem
#include <math.h>

using namespace std;

#include "glad.h"
#include <GLFW/glfw3.h>

//vector
#include <vector>

const float pi = 3.14159;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();
int createCircle(int nPoints, float radius = 0.15);

const GLuint WIDTH = 800, HEIGHT = 800;

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
	    //-> Triângulo da Pizza
		//T0
		//x    y    z
		0.2,  0.4, 0.0, //v0
		0.8,  0.1, 0.0, //v1
		0.8, 0.7, 0.0, //v2
			  
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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 6 -- Henrique", NULL, NULL);
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
	GLint colorLoc2 = glGetUniformLocation(shaderID, "inputColor");
	GLint colorLoc3 = glGetUniformLocation(shaderID, "inputColor");
	
	glUseProgram(shaderID);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);

		GLfloat x = -.5f; GLfloat y = .5f; GLfloat radius = .4f;
        int nVertices = 8;

        GLfloat twicePi = 2.0 * pi;

        glUniform4f(colorLoc3, 0.78f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x, y);
            for (int i = 0; i <= nVertices; i++) {
                glVertex2f(
                    x + (radius * cos(i * twicePi / nVertices)),
                    y + (radius * sin(i * twicePi / nVertices))
                );
            }
        glEnd();

		x = -.5f; y = -.5f; radius = .4f;
        nVertices = 5;

        glUniform4f(colorLoc3, 0.78f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x, y);
            for (int i = 0; i <= nVertices; i++) {
                glVertex2f(
                    x + (radius * cos(i * twicePi / nVertices)),
                    y + (radius * sin(i * twicePi / nVertices))
                );
            }
        glEnd();

		x = .5f; y = -.5f; radius = .4f;
        nVertices = 20;

        glUniform4f(colorLoc3, 0.78f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x, y);
            for (int i = 0; i < nVertices; i++) {
                glVertex2f(
                    x + (radius * cos(i * twicePi / nVertices)),
                    y + (radius * sin(i * twicePi / nVertices))
                );
            }
        glEnd();

		glUniform4f(colorLoc, 0.34f, 0.33f, 0.10f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

