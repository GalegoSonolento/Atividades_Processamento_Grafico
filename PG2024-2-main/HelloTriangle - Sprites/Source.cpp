/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 13/08/2024
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB_IMAGE
#include <stb_image.h>

using namespace glm;

float cameraOffsetX = 0.0f;
float characterSpeed = 5.0f; // Velocidade constante do personagem

// Variáveis globais para controle do pulo
bool isJumping = false;
bool isJumpKeyHeld = false;
float jumpVelocity = 0.0f;
float jumpStrength = 5.0f; // Força base do pulo
float gravity = -9.8f;
float groundLevel = 200.0f; // Posição y no solo
float jumpHoldTime = 0.0f; // Tempo que a tecla de pulo foi pressionada
float maxJumpHoldTime = 0.5f; // Tempo máximo que a tecla pode ser segurada para aumentar o pulo

// Variáveis de pontuação
int score = 0; // Pontuação inicial
bool passedObstacle1 = false; // Controle para evitar múltiplas pontuações
bool passedObstacle2 = false;
bool passedObstacle3 = false;
bool passedObstacle4 = false;


// Protótipo da função para atualizar o pulo
void updateJump(float deltaTime);

struct Sprite
{
	GLuint VAO;
	GLuint texID;
	vec3 position;
	vec3 dimensions;
	float angle;
	// Para controle da animação
	int nAnimations, nFrames;
	int iAnimation, iFrame;
	vec2 d;
	float FPS;
	float lastTime;

	// Função de inicialização
	void setupSprite(int texID, vec3 position, vec3 dimensions, int nFrames, int nAnimations);
};

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

bool checkCollision(Sprite& character, Sprite& obstacle);

// Protótipos das funções
int setupShader();
int loadTexture(string filePath, int &imgWidth, int &imgHeight);
void drawSprite(Sprite spr, GLuint shaderID);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = "#version 400\n"
								   "layout (location = 0) in vec3 position;\n"
								   "layout (location = 1) in vec2 texc;\n"
								   "uniform mat4 projection;\n"
								   "uniform mat4 model;\n"
								   "out vec2 texCoord;\n"
								   "void main()\n"
								   "{\n"
								   //...pode ter mais linhas de código aqui!
								   "gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);\n"
								   "texCoord = vec2(texc.s, 1.0 - texc.t);\n"
								   "}\0";

// Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = "#version 400\n"
									 "in vec2 texCoord;\n"
									 "uniform sampler2D texBuffer;\n"
									 "uniform vec2 offsetTex;\n"
									 "out vec4 color;\n"
									 "void main()\n"
									 "{\n"
									 "color = texture(texBuffer, texCoord + offsetTex);\n"
									 "}\n\0";

float vel = 1.2;

bool keys[1024] = {false};

Sprite background1, background2, background3, background4, character, obstacle, obstacle2, obstacle3, obstacle4;

int main()
{
    // Inicialização da GLFW, GLAD e outros componentes da OpenGL (já existentes no seu código)
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Sprites!", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint shaderID = setupShader();

    // Inicializando dois sprites de fundo
    int imgWidth, imgHeight;
    int texID = loadTexture("../Texturas/backgrounds/PNG/Battleground3/Bright/Battleground3.png", imgWidth, imgHeight);
    float backgroundWidth = imgWidth * 0.5f;

    // Background 1
    background1.setupSprite(texID, vec3(400.0, 300.0, 0.0), vec3(backgroundWidth, imgHeight * 0.5f, 1.0), 1, 1);
    
    // Background 2 (colocado logo à direita do Background 1)
    background2.setupSprite(texID, vec3(400.0 + backgroundWidth, 300.0, 0.0), vec3(backgroundWidth, imgHeight * 0.5f, 1.0), 1, 1);

	//background3 (mais à direita do 2 ainda)
	background3.setupSprite(texID, vec3(400.0 + (2 * backgroundWidth), 300.0, 0.0), vec3(backgroundWidth, imgHeight * 0.5f, 1.0), 1, 1);

	//background4 (mais à direita do 3 ainda)
	background4.setupSprite(texID, vec3(400.0 + (3 * backgroundWidth), 300.0, 0.0), vec3(backgroundWidth, imgHeight * 0.5f, 1.0), 1, 1);

    // Inicializando a sprite do personagem
    texID = loadTexture("../Texturas/characters/PNG/1 Pink_Monster/Pink_Monster_Walk_6.png", imgWidth, imgHeight);
    character.setupSprite(texID, vec3(50.0, 200.0, 0.0), vec3(imgWidth / 6.0 * 2.0, imgHeight * 2.0, 1.0), 6, 1);

    texID = loadTexture("../Texturas/characters/PNG/1 Pink_Monster/Pink_Monster_Walk_6.png", imgWidth, imgHeight);

    obstacle.setupSprite(texID, vec3(350.0, 200.0, 0.0), vec3(imgWidth / 6.0 * 2.0, imgHeight * 2.0, 1.0), 6, 1);
    obstacle2.setupSprite(texID, vec3(400.0 + backgroundWidth/2, 200.0, 0.0), vec3(imgWidth / 6.0 * 2.0, imgHeight * 2.0, 1.0), 6, 1);
    obstacle3.setupSprite(texID, vec3(400.0 + 2 * backgroundWidth/2, 200.0, 0.0), vec3(imgWidth / 6.0 * 2.0, imgHeight * 2.0, 1.0), 6, 1);
    obstacle4.setupSprite(texID, vec3(400.0 + 3 * backgroundWidth/2, 200.0, 0.0), vec3(imgWidth / 6.0 * 2.0, imgHeight * 2.0, 1.0), 6, 1);

    glUseProgram(shaderID);
    mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    // Loop principal
	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calcula o tempo delta entre frames
        float now = glfwGetTime();
        float deltaTime = now - character.lastTime;
        character.lastTime = now;

        // Movimentação do personagem
        character.position.x += characterSpeed;

        // Atualiza o deslocamento da câmera
        cameraOffsetX = character.position.x - 100.0f;

        // Atualiza o pulo
        updateJump(deltaTime);

        // Atualiza a matriz de projeção para seguir o personagem
        mat4 projection = ortho(cameraOffsetX, cameraOffsetX + 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

				// Verifica se o personagem passou pelos obstáculos sem colidir e incrementa a pontuação
        if (character.position.x > obstacle.position.x && !passedObstacle1 && !isJumping) {
            score++;
            passedObstacle1 = true;
            std::cout << "Pontuação: " << score << std::endl;
        }
        if (character.position.x > obstacle2.position.x && !passedObstacle2 && !isJumping) {
            score++;
            passedObstacle2 = true;
            std::cout << "Pontuação: " << score << std::endl;
        }
        if (character.position.x > obstacle3.position.x && !passedObstacle3 && !isJumping) {
            score++;
            passedObstacle3 = true;
            std::cout << "Pontuação: " << score << std::endl;
        }
        if (character.position.x > obstacle4.position.x && !passedObstacle4 && !isJumping) {
            score++;
            passedObstacle4 = true;
            std::cout << "Pontuação: " << score << std::endl;
        }

		// Verifica colisões com os obstáculos
		if (checkCollision(character, obstacle) || 
			checkCollision(character, obstacle2) ||
			checkCollision(character, obstacle3) ||
			checkCollision(character, obstacle4)) {
			std::cout << "Game Over: Collision Detected!" << std::endl;
			glfwSetWindowShouldClose(window, GL_TRUE);  // Fecha a janela e encerra o jogo
		}

        // Desenho dos fundos (backgrounds)
        drawSprite(background1, shaderID);
        drawSprite(background2, shaderID);
        drawSprite(background3, shaderID);
        drawSprite(background4, shaderID);
        drawSprite(obstacle, shaderID);
		drawSprite(obstacle2, shaderID);
		drawSprite(obstacle3, shaderID);
		drawSprite(obstacle4, shaderID);
		drawSprite(character, shaderID);

        // Reposicionamento do fundo para efeito de loop
        if (background1.position.x + backgroundWidth / 2.0f < cameraOffsetX)
        {
            background1.position.x = background4.position.x + backgroundWidth;
        }
        if (background2.position.x + backgroundWidth / 2.0f < cameraOffsetX)
        {
            background2.position.x = background1.position.x + backgroundWidth;
        }
        if (background3.position.x + backgroundWidth / 2.0f < cameraOffsetX)
        {
            background3.position.x = background2.position.x + backgroundWidth;
        }
        if (background4.position.x + backgroundWidth / 2.0f < cameraOffsetX)
        {
            background4.position.x = background3.position.x + backgroundWidth;
        }

        if (obstacle.position.x + backgroundWidth / 1.5 < cameraOffsetX)
        {
            obstacle.position.x = obstacle4.position.x + backgroundWidth / 1.5;
        }
        if (obstacle2.position.x + backgroundWidth / 1.5 < cameraOffsetX)
        {
            obstacle2.position.x = obstacle.position.x + backgroundWidth / 1.5;
        }
        if (obstacle3.position.x + backgroundWidth / 1.5 < cameraOffsetX)
        {
            obstacle3.position.x = obstacle2.position.x + backgroundWidth / 1.5;
        }
        if (obstacle4.position.x + backgroundWidth / 1.5 < cameraOffsetX)
        {
            obstacle4.position.x = obstacle3.position.x + backgroundWidth / 1.5;
        }

        // Atualização da animação do personagem
        if (deltaTime >= 1.0 / character.FPS) {
            character.iFrame = (character.iFrame + 1) % character.nFrames;
            character.lastTime = now;
        }
        vec2 offsetTex = vec2(character.iFrame * character.d.s, 0.0);
        glUniform2f(glGetUniformLocation(shaderID, "offsetTex"), offsetTex.s, offsetTex.t);
        drawSprite(character, shaderID);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

bool checkCollision(Sprite& character, Sprite& obstacle) {
    float charLeft = character.position.x - character.dimensions.x / 2.0f;
    float charRight = character.position.x + character.dimensions.x / 2.0f;
    float charBottom = character.position.y - character.dimensions.y / 2.0f;
    float charTop = character.position.y + character.dimensions.y / 2.0f;

    float obsLeft = obstacle.position.x - obstacle.dimensions.x / 2.0f;
    float obsRight = obstacle.position.x + obstacle.dimensions.x / 2.0f;
    float obsBottom = obstacle.position.y - obstacle.dimensions.y / 2.0f;
    float obsTop = obstacle.position.y + obstacle.dimensions.y / 2.0f;

    // Verifica se os retângulos estão sobrepostos
    if (charRight >= obsLeft && charLeft <= obsRight &&
        charTop >= obsBottom && charBottom <= obsTop) {
        return true;  // Colisão detectada
    }

    return false;
}

// Função para gerenciar o pulo com base no tempo que a tecla é pressionada
void updateJump(float deltaTime) {
    if (isJumpKeyHeld && !isJumping) {
        // Incrementa o tempo que a tecla está pressionada
        jumpHoldTime += deltaTime;
        // Limita o tempo de seguramento
        if (jumpHoldTime > maxJumpHoldTime) {
            jumpHoldTime = maxJumpHoldTime;
        }
    }

    if (isJumping) {
        // Atualiza a velocidade com a gravidade
        jumpVelocity += gravity * deltaTime;
        character.position.y += jumpVelocity;

        // Se o personagem tocar o solo, termina o pulo
        if (character.position.y <= groundLevel) {
            character.position.y = groundLevel;
            isJumping = false;
            jumpVelocity = 0.0f;
            jumpHoldTime = 0.0f; // Reseta o tempo de pressão
        }
    }
}


// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
// Função de callback de teclado - detectar espaço para pulo
// Função de callback de teclado - detectar espaço para pulo
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS && !isJumping) {
            isJumpKeyHeld = true;
        }

        if (action == GLFW_RELEASE && !isJumping) {
            isJumpKeyHeld = false;
            // Iniciar o pulo ao soltar a tecla espaço
            isJumping = true;
            // Ajusta a força do pulo com base no tempo que a tecla foi pressionada
            jumpVelocity = jumpStrength + (jumpHoldTime * 10.0f); // Multiplicador para ajustar a força
        }
    }
}

// Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no iniçio deste arquivo
//  A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
void Sprite::setupSprite(int texID, vec3 position, vec3 dimensions, int nFrames, int nAnimations)
{
	this->texID = texID;
	this->dimensions = dimensions;
	this->position = position;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;
	iAnimation = 0;
	iFrame = 0;

	d.s = 1.0 / (float)nFrames;
	d.t = 1.0 / (float)nAnimations;
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		// x   y     z    s     		t
		// T0
		-0.5, -0.5, 0.0, 0.0, 0.0, // V0
		-0.5, 0.5, 0.0, 0.0, d.t,  // V1
		0.5, -0.5, 0.0, d.s, 0.0,  // V2
		0.5, 0.5, 0.0, d.s, d.t	   // V3

	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	// Atributo 0 - Posição - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo 1 - Coordenadas de textura - s, t
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	this->VAO = VAO;
	FPS = 12.0;
	lastTime = 0.0;
}

int loadTexture(string filePath, int &imgWidth, int &imgHeight)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Configurando o wrapping da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Configurando o filtering de minificação e magnificação da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Carregamento da imagem da textura
	int nrChannels;
	unsigned char *data = stbi_load(filePath.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << filePath << std::endl;
	}

	return texID;
}

void drawSprite(Sprite spr, GLuint shaderID)
{
	glBindVertexArray(spr.VAO); // Conectando ao buffer de geometria

	glBindTexture(GL_TEXTURE_2D, spr.texID); // conectando o buffer de textura

	// Matriz de modelo - Tranformações na geometria, nos objetos
	mat4 model = mat4(1); // matriz identidade
	// Translação
	model = translate(model, spr.position);
	// Rotação
	model = rotate(model, radians(spr.angle), vec3(0.0, 0.0, 1.0));
	// Escala
	model = scale(model, spr.dimensions);
	// Enviar para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

	// Chamada de desenho - drawcall
	// Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0); // Desconectando o buffer de geometria
}
