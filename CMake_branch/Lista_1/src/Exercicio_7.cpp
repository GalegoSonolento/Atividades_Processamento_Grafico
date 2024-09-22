// Código pra criação de espiral

#include <iostream>
#include <string>
#include <assert.h>
#include <math.h>

using namespace std;

#include "glad.h"
#include <GLFW/glfw3.h>

#include <vector>

const float pi = 3.14159;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();
// Aqui o raio foi substituído por espaçamento (entre as linhas)
int createSpiral(int nPoints, float spacing = 0.01);

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
		//x     y    z
		//T0
		0.0,  0.0,  0.0, //v0
		0.9,  0.9,  0.0, //v1
 		0.9,  -0.9, 0.0  //v2
			  
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

int createSpiral(int nPoints, float spacing) {
	
	vector <GLfloat> vertices;

    float angle_multiplier = 0.1;
	float angle = 0.0;
	float slice = 2 * pi / (float)nPoints;

	//adicionar ponto de origem (0, 0, 0) - centro do circulo
	vertices.push_back(0.0); // Xc
	vertices.push_back(0.0); // Yc
	vertices.push_back(0.0); // Zc

	for (int i = 0; i <= nPoints; i++) {
        // a cada iteração o ângulo é calculado novamente pra que aumente
        angle = i * angle_multiplier; // aumente ou diminua o valor dessa multiplicação para espirais mais apertados ou mais abertos
		float x = spacing * angle * cos(angle); // multiplique espaçamento pelo ângulo e pelo cálculo padrão de circunferência
		float y = spacing * angle * sin(angle);
		float z = 0.0;
        /*
        Here, as angle increases, the product spacing * angle determines how far the point is from the origin (0,0) at that specific angle. So, the spiral will get progressively larger as you increase the angle.
        */

		vertices.push_back(x); // x
		vertices.push_back(y); // y
		vertices.push_back(z); // z

		angle = angle + slice;
	}

	//configuração dos buffers VBO e VAO

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

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

    int nPoints = 2000;

	GLuint VAO = createSpiral(nPoints);

    int nVertices = nPoints + 2;

	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	
	glUseProgram(shaderID);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);

        glUniform4f(colorLoc, 0.78f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINE_STRIP, 0, nVertices / 2);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

