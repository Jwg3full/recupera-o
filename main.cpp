#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include "gl_utils.h"
#include "Layer.h"
#include "Triangulo.h"

using namespace std;

// Definição do tamanho da janela do sistema operacional
int larguraJanela = 800;
int alturaJanela = 600;

GLFWwindow *janela = NULL;

int main()
{
    // Prepara o GLFW e cria a janela grafica
    gl_utils::criar_janela();
    glfwSwapInterval(1); // Ativa o V-Sync para limitar o FPS à taxa de atualização do monitor
    glfwSetWindowAttrib(janela, GLFW_RESIZABLE, GLFW_FALSE); // bloqueia o redimensionamento da janela

    
    // Coordenadas (X, Y, Z) que formam um triângulo simples 
    GLfloat points_triangulo[] = {
        -0.1f, -0.1f, 0.0f, // v0: Ponto inferior esquerdo
         0.1f, -0.1f, 0.0f, // v1: Ponto inferior direito
         0.0f,  0.1f, 0.0f  // v2: Ponto superior central
    };

    // Coordenadas para formar o Quadrado do Cenário de Fundo (X, Y, U, V)
    // Contém a posição na tela e a posição mapeada da textura da imagem
    float vertices[] = {
         1.0f,  0.727f, 1.0f, 1.0f, // top right (Canto superior direito)
         1.0f, -0.727f, 1.0f, 0.0f, // bottom right (Canto inferior direito)
        -1.0f, -0.727f, 0.0f, 0.0f, // bottom left (Canto inferior esquerdo)
        -1.0f,  0.727f, 0.0f, 1.0f, // top left (Canto superior esquerdo)
    };

    // Diz a OpenGL como ligar os 4 vértices acima para formar 2 triângulos (um quadrado)
    GLuint indices[] = {
        2, 1, 0, // Primeiro triângulo
        0, 3, 2  // Segundo triângulo
    };

    printf("Carregando mapa \n");
    
    
    // Aloca a classe Layer na memória Heap e envia a geometria do fundo para a GPU
    Layer* layers = new Layer(vertices, sizeof(vertices), indices, sizeof(indices));
    
    // Adiciona as imagens do fundo. O Z positivo (ex: 0.55) significa que está longe da câmera
    // O ratex (ex: 0.0001f) define a velocidade de movimento daquela camada no paralaxe
    layers->adicionar_layer(0.55f, 0.0001f, "assets/w0.png");
    layers->adicionar_layer(0.54f, 0.003f,  "assets/w1.png");
    layers->adicionar_layer(0.53f, 0.06f,   "assets/w2.png");
    layers->adicionar_layer(0.52f, 0.1f,    "assets/w3.png");
    layers->adicionar_layer(0.51f, 0.8f,    "assets/w4.png");
    layers->adicionar_layer(0.50f, 0.8f,    "assets/w5.png");
    layers->criar_program_shader(); // Compila os shaders do cenário

    // Cria o personagem principal na memória Heap e envia seus pontos para a GPU
    Triangulo* personagem = new Triangulo();
    personagem->criar_triangulo(points_triangulo, sizeof(points_triangulo));
    personagem->criar_program_shader();

    //novo🐵🐵🐵
    // Cria o objeto que vai viajar na tela
    Triangulo* objetoZ = new Triangulo();
    objetoZ->criar_triangulo(points_triangulo, sizeof(points_triangulo));
    objetoZ->criar_program_shader();
    objetoZ->set_posicao(0.0f, 0.4f, 0.6f);// Define a posição inicial do objeto: nasce no fundo (Z = 0.6f), atrás de todas as camadas
    
    // Habilita o Teste de Profundidade. o openGL vai comparar a profundidade de cada pixel desenhado e decidir se ele deve aparecer ou ser ocultado por outro pixel mais próximo da camera
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LEQUAL);//faz com que objetos mais próximos da câmera cubram os objetos mais distantes, criando a ilusão de profundidade
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    


   
    // mainloop
    while (!glfwWindowShouldClose(janela))
    {

        // Define a cor de fundo e limpa as memórias de Cor e de Profundidade
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // manda para a OpenGL qual o tamanho da área de desenho na janela
        glViewport(0, 0, larguraJanela, alturaJanela);

        //novo🐵🐵🐵
        // Pega a profundidade atual do objeto e subtrai para trazê-lo em direção à tela
        float currentZ = objetoZ->get_posZ();
        objetoZ->set_posZ(currentZ - 0.002f); 

        // Se o objeto passou da câmera (Z < 0), ele reaparece no fundo do horizonte (Z = 0.6f) com uma posição horizontal random
        if (objetoZ->get_posZ() < -0.8f) 
        {
            // Sorteia uma posição horizontal random e manda ele para o fundo do "horizonte"'
            float posX_aleatorio = (rand() % 200 - 100) / 100.0f; 
            objetoZ->set_posicao(posX_aleatorio, 0.4f, 0.6f);
        }

        // Chama as funções que enviam o comando de pintura para a GPU
        layers->carregar_layer();           // Pinta todas as 6 texturas do fundo
        objetoZ->carregar_triangulo();      // Pinta o triângulo que viaja
        personagem->carregar_triangulo();   // Pinta o jogador
        //novo🐵🐵🐵

        // Processa mause/teclado
        glfwPollEvents();
        
        //  fecha a janela se apertar ESC
        if (GLFW_PRESS == glfwGetKey(janela, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(janela, 1);
        }

        // Sistema de Movimento e Paralaxe
        if (GLFW_PRESS == glfwGetKey(janela, GLFW_KEY_A)) // esquerda rapido
        {
            layers->mover_esquerda(); 
            objetoZ->add_posicaoX(0.1f * (1.0f - objetoZ->get_posZ())); // 🐵velocidade lateral 
        }
        else if (GLFW_PRESS == glfwGetKey(janela, GLFW_KEY_D)) // direita lento
        {
            layers->mover_direita();
            objetoZ->add_posicaoX(-0.001f * (1.0f - objetoZ->get_posZ())); 
        }
        else if (GLFW_PRESS == glfwGetKey(janela, GLFW_KEY_W)) // cima
        {
            personagem->set_posicaoY(0.01);
        }
        else if (GLFW_PRESS == glfwGetKey(janela, GLFW_KEY_S)) // baixo
        {
            personagem->set_posicaoY(-0.01);
        }
        else
        {
            layers->parado(); 
        }

        // Troca o buffer 
        glfwSwapBuffers(janela);
    }

    // Limpa a memory do sistema antes do programa terminar
    glfwTerminate();
    return 0;
}