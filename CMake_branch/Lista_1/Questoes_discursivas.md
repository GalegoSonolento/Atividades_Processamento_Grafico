1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?
    - GLSL é a biblioteca do OpenGL relacionada especificamente com Shaders - uma lingugem para shaders. Nesse sentido, os dois tipos de shaders orbigatórios a estarem presentes na pipeline são "Vertex" e "Fragment".
 
2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?
    - As primitivas gráficas são os buffers enviados para processamento à partir de uma chamada "drawcall". Nesse sentido, em OpenGL, podemos fazer o armazenamento dos vértices através de Vertex Buffer Objects (VBOs) - que é basicamente um array com todos os vértices.
 
3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).
    - VBO - Vertex Buffer Objetcs - Arrays de dados usualmente floats com as informações de vértices da figura.
    - VAO - Vertex Array Objects - outro array com as informações de todos os vértices streamalizados para o desenho das arestas. Utiliza as informações do VBO e pode conter mais informações, como valores de cores RGBA
 
4. Analise o código fonte do projeto Hello Triangle. Localize e relacione os conceitos de shaders, VBOs e VAO apresentados até então. Não precisa entregar nada neste exercício.
 
5. Faça o desenho de 2 triângulos na tela. Desenhe eles:
    a. Apenas com o polígono preenchido
    b. Apenas com contorno
    c. Apenas como pontos
    d. Com as 3 formas de desenho juntas

6. 