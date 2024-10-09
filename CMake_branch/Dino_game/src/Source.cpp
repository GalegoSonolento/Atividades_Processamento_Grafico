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
#include "glad.h"

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include "glm/glm/glm.hpp" 
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace glm;

// Protótipos das funções
void verificarColisao(); // Ajuste o tipo de retorno e os parâmetros conforme necessário
void desenharDinossauro();
void atualizarDinossauro(float deltaTime);

// Variáveis globais para o dinossauro
bool pulando = false;
float velocidadeY = 0.0f;
float posY = 300.0f;  // Posição inicial no eixo Y
float gravidade = -1000.0f;
float impulsoPulo = 500.0f;
bool jogoIniciado = false;

// Protótipo da função de callback de teclado
// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
//"gl_Position = projection * vec4(position.x, position.y, position.z, 1.0);\n"
//"}\0";
"gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";


//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 400\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

// Função que desenha o dinossauro (um quadrado, por exemplo)
void desenharDinossauro(GLuint shaderID, GLuint VAO, GLint colorLoc, vec2 posDino) {
    // Usamos a função de translação para posicionar o dinossauro
    mat4 model = mat4(1);
    model = translate(model, vec3(posDino.x, posDino.y, 0.0f));
    model = scale(model, vec3(50.0f, 50.0f, 1.0f));  // Tamanho do dinossauro
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);  // Cor do dinossauro (verde)
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);  // Supondo que você está desenhando triângulos
    glBindVertexArray(0);
}

// Função que desenha os obstáculos (cactos)
void desenharObstaculo(GLuint shaderID, GLuint VAO, GLint colorLoc, vec2 posObstaculo) {
    mat4 model = mat4(1);
    model = translate(model, vec3(posObstaculo.x, posObstaculo.y, 0.0f));
    model = scale(model, vec3(30.0f, 70.0f, 1.0f));  // Tamanho do cacto
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);  // Cor do obstáculo (vermelho)
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

// Função para atualizar a posição do dinossauro
void atualizarDinossauro(float deltaTime) {
    if (pulando) {
        velocidadeY += gravidade * deltaTime;
        posY += velocidadeY * deltaTime;

        // Se atingir o solo, parar o pulo
        if (posY <= 300.0f) {
            posY = 300.0f;
            pulando = false;
            velocidadeY = 0.0f;
        }
    }
}

// Variável global para a posição do obstáculo
vec2 posObstaculo = vec2(800.0f, 300.0f);  // Começa fora da tela à direita

// Função que atualiza a posição dos obstáculos
void atualizarObstaculo(float deltaTime) {
    posObstaculo.x -= 200.0f * deltaTime;  // Velocidade de movimento do obstáculo

    // Se o obstáculo sair da tela, reposiciona
    if (posObstaculo.x < -50.0f) {
        posObstaculo.x = 800.0f;
    }
}

bool verificarColisao(vec2 posDino, vec2 posObstaculo) {
    return (posDino.x < posObstaculo.x + 30.0f && posDino.x + 50.0f > posObstaculo.x &&
            posDino.y < posObstaculo.y + 70.0f && posDino.y + 50.0f > posObstaculo.y);
}


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modificação de janela -- Henrique", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();
	
	glUseProgram(shaderID);

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

	//Exercicio 2
	//mat4 projection = ortho(-10.0f,10.0f,-10.0f,10.0f,-1.0f,1.0f);
	mat4 projection = ortho(0.0f,800.0f,600.0f,0.0f,-1.0f,1.0f);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

    //Matriz de modelo - Tranformações na geometria, nos objetos
	mat4 model = mat4(1); //matriz identidade
	//Translação
	model = translate(model,vec3(400.0, 300.0, 0.0));
	//Rotação
	//model = rotate (model,radians(45.0f), vec3(0.0,0.0,1.0));
	
	//Escala
	model = scale(model,vec3(300.0, 300.0, 1.0));

	//Enviar para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    
    while (!glfwWindowShouldClose(window)) {
        // Limpa o buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Verifica se o jogo já começou
        if (!jogoIniciado) {
            // Exibe mensagem de "Pressione Espaço para Iniciar" (essa parte vai depender do seu sistema de renderização)
            cout << "Pressione Espaço para Iniciar o Jogo!" << endl;

            // Checa se o jogador pressionou a tecla espaço
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                jogoIniciado = true;
            }
        } else {
            // Checa se houveram eventos de input
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Atualiza a posição do dinossauro e dos obstáculos
        float deltaTime = glfwGetTime();  // Usar para calcular o tempo entre quadros
        atualizarDinossauro(deltaTime);
        atualizarObstaculo(deltaTime);

        // Desenha o dinossauro e o obstáculo
        desenharDinossauro(shaderID, VAO, colorLoc, vec2(100.0f, posY));
        desenharObstaculo(shaderID, VAO, colorLoc, posObstaculo);

        // Verifica colisão
        if (verificarColisao(vec2(100.0f, posY), posObstaculo)) {
            cout << "Game Over! Colisão detectada!" << endl;
            break;  // Termina o jogo se colidir
        }
        }

        // Troca os buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	/*
        // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window)) {
        // Checa se houveram eventos de input
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Atualiza a posição do dinossauro e dos obstáculos
        float deltaTime = glfwGetTime();  // Usar para calcular o tempo entre quadros
        atualizarDinossauro(deltaTime);
        atualizarObstaculo(deltaTime);

        // Desenha o dinossauro e o obstáculo
        desenharDinossauro(shaderID, VAO, colorLoc, vec2(100.0f, posY));
        desenharObstaculo(shaderID, VAO, colorLoc, posObstaculo);

        // Verifica colisão
        if (verificarColisao(vec2(100.0f, posY), posObstaculo)) {
            cout << "Game Over! Colisão detectada!" << endl;
            break;  // Termina o jogo se colidir
        }

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    */
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !pulando) {
        pulando = true;
        velocidadeY = impulsoPulo;
    }
}

//Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
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
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
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
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		/*
		//x   y     z
		//T0
		-0.5 * 300 + 400, -0.5 * 300 + 300, 0.0, //v0
		 0.5 * 300 + 400, -0.5 * 300 + 300, 0.0, //v1
 		 0.0 * 300 + 400,  0.5 * 300 + 300, 0.0, //v2
		//T1
        */
		//x   y     z
		//T0 -> lembrar que nesse caso o eixo y está invertido por causa da ortogonalidade
		-0.5,  0.5, 0.0, //v0
		 0.5,  0.5, 0.0, //v1
 		 0.0, -0.5, 0.0, //v2
		//T1
			  
	};

	GLuint VBO, VAO;
	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}