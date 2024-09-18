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

/* -> Código pra criar o Pac-man
//const int steps = 1000;
// para fazer um círculo propriamente dito, escrevo 1000 triângulos em circunferência, assim ele fica smooth

//const int steps = 8; // para fazer octógono

const int steps = 100;
const float angle = 3.1415926 * 2.f / steps;
*/

/*
//-> Código pra rodela da pizza
const int steps_pizza = 100;
const float angle_pizza = 3.1415926 * 2.f / steps_pizza;
*/

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();
int createCircle(int nPoints, float radius = 0.5);

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
		/* -> Vértices do Pac-Man
		//x     y    z
		//T0
		0.0,  0.0,  0.0, //v0
		0.9,  0.9,  0.0, //v1
 		0.9,  -0.9, 0.0  //v2
		*/
	    //-> Triângulo da Pizza
		//T0
		//x    y    z
		-0.8,  0.0, 0.0, //v0
		 0.7,  0.7, 0.0, //v1
		 0.7, -0.7, 0.0, //v2
		//T1
		//x    y    z
		-0.8,  0.0, 0.0, //v0
		 0.93, 0.8, 0.0, //v1
		 0.93,-0.8, 0.0  //v2
			  
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

int createCircle(int nPoints, float radius) {
	
	vector <GLfloat> vertices;


	float angle = 0.0;
	float slice = 2 * pi / (float)nPoints;

	//adicionar ponto de origem (0, 0, 0) - centro do circulo
	vertices.push_back(0.0); // Xc
	vertices.push_back(0.0); // Yc
	vertices.push_back(0.0); // Zc

	for (int i = 0; i < nPoints; i++) {
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		float z = 0.0;

		vertices.push_back(x); // x
		vertices.push_back(y); // y
		vertices.push_back(z); // z

		angle = angle + slice;
	}

	//configuração dos buffers VBO e VAO

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	GLint colorLoc2 = glGetUniformLocation(shaderID, "inputColor");
	GLint colorLoc3 = glGetUniformLocation(shaderID, "inputColor");
	
	glUseProgram(shaderID);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//glClearColor(0.95f, 0.70f, 0.10f, 1.0f); //-> cor de fundo amarela
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* -> Código pra criar o Pac-Man
		float xPos = 0; float yPos = 0; float radius = 0.9f;

		float prevX = xPos;
		float prevY = yPos;

		for (int i = 0; i <= steps; i++) {
			
			float newX = radius * sin(angle*i);
			float newY = -radius * cos(angle*i);


			glBegin(GL_TRIANGLES);
			//glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(prevX, prevY, 0.0f);
			glVertex3f(newX, newY, 0.0f);
			glEnd();

			prevX = newX;
			prevY = newY;

			glUniform4f(colorLoc, 0.07f, 0.23f, 0.34f, 1.0f);
		}

		glBindVertexArray(VAO);

		glUniform4f(colorLoc2, 0.95f, 0.70f, 0.10f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		*/

		glBindVertexArray(VAO);

		glUniform4f(colorLoc2, 0.34f, 0.33f, 0.10f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 3, 6);

		glUniform4f(colorLoc, 0.95f, 0.70f, 0.10f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//-> Rodela da pizza
		/*
		float xPos_pizza = 0; float yPos_pizza = 0; float radius_pizza = 0.18f;

		float prevX_pizza = xPos_pizza;
		float prevY_pizza = yPos_pizza;

		for (int i = 0; i <= steps_pizza; i++) {
			
			float newX_pizza = radius_pizza * sin(angle_pizza*i);
			float newY_pizza = -radius_pizza * cos(angle_pizza*i);


			glBegin(GL_TRIANGLES);
			//glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(prevX_pizza, prevY_pizza, 0.0f);
			glVertex3f(newX_pizza, newY_pizza, 0.0f);
			glEnd();

			prevX_pizza = newX_pizza;
			prevY_pizza = newY_pizza;

			glUniform4f(colorLoc3, 0.78f, 0.0f, 0.0f, 1.0f);
		}
		*/



		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

