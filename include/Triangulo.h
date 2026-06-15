#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

// Representa um triângulo 
class Triangulo{

    public:
        Triangulo();
        // Cria buffers (VBO/VAO) a partir de um array de pontos
        void criar_triangulo(float* points, size_t tamanho_points);
        // Envia dados para GPU e desenha o triângulo usando seu shader
        void carregar_triangulo();
        // Compila/usa o shader específico dos sprites
        void criar_program_shader();
        // Move verticalmente o triângulo (ajusta posicao.y)
        void set_posicaoY(float y);
        // Define posição X,Y,Z do triângulo (Z usado para escala/profundidade)
        void set_posicao(float x, float y, float z);
        // Ajusta X (usado para efeito de paralaxe ao mover camadas)
        void add_posicaoX(float x); // Para o efeito de paralaxe
        // Define a coordenada Z
        void set_posZ(float z);
        // Retorna a coordenada Z atual🐵
        float get_posZ();

    private:
        GLuint vao, vbo;           // Objetos OpenGL para armazenar o triângulo
        glm::vec2 posicao;         // Posição X,Y na tela
        float posZ = 0.0f;        // Profundidade Z afeta escala🐵
        glm::vec3 color;           // Cor RGB do triângulo
        const char* fShader;
        const char* vShader;
        Shader shader_programme;   // Shader usado para desenhar o triângulo

        // Gera cor aleatória para o triângulo
        void gerar_cor();
        // Calcula transformações (model matrix) e envia para o shader
        void mover_triangulo();


};