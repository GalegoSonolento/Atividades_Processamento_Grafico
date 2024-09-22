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
int createCircle(int nPoints, GLfloat radius = .5f);

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

int createCircle(int nPoints, GLfloat radius) {
	
	vector <GLfloat> vertices;


	float angle = 0.0;
	GLfloat slice = 2 * pi / (float)nPoints;

	//adicionar ponto de origem (0, 0, 0) - centro do circulo
    GLfloat x=-.3f; GLfloat y=-.3f; 

	vertices.push_back(x); // Xc
	vertices.push_back(y); // Yc
	// vertices.push_back( 0.0); // Zc

	for (int i = 0; i <= nPoints; i++) {
		x + (radius * cos(i * slice));
		y + (radius * sin(i * slice));
		//float z = 0.0;

		vertices.push_back(x); // x
		vertices.push_back(y); // y
		//vertices.push_back(z); // z

		//angle = angle + slice;
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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 9 - Pizza! -- Henrique", NULL, NULL);
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

	//criando o círculo -> Puramente criação do cículo da calabresa da pizza
	int nPoints = 2000;

	GLuint VAO2 = createCircle(nPoints);

	int nVertices = nPoints + 2; // inclui o centro e o extra (repetição do primeiro)

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

		//-> Pedaços da pizza
		glUniform4f(colorLoc2, 0.34f, 0.33f, 0.10f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 3, 6);

		glUniform4f(colorLoc, 0.95f, 0.70f, 0.10f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//-> Rodela da pizza
		GLfloat x = -.3f; GLfloat y = .0f; GLfloat radius = .2f;
        int nVertices = 40;

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

        x = .3f; y = .2f; radius = .25f;
        nVertices = 40;

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

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}