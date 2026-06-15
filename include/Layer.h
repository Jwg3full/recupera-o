#include <vector>
#include <GL/glew.h>
#include "stb_image.h"
#include "gl_utils.h"
#include "Shader.h"

using namespace std;

// Classe que gerencia várias camadas (layers) para criar um efeito de parallax
class Layer
{

public:
    // Constrói o objeto Layer usando um conjunto de vértices e índices (quad)
    Layer(float *vertices, int size_vertices, unsigned int *indices, int size_indices);
    // Adiciona uma nova camada com profundidade Z, taxa de deslocamento X e arquivo de textura
    void adicionar_layer(float z, float ratex, const char* filename);
    // Renderiza todas as camadas
    void carregar_layer();
    // Ajusta a taxa de parallax para movimento à direita
    void mover_direita();
    // Ajusta a taxa de parallax para movimento à esquerda
    void mover_esquerda();
    // Para o deslocamento (nenhum parallax)
    void parado();
    // Compila/ativa o programa de shader usado pelas camadas
    void criar_program_shader();

private:
    // Estrutura interna para manter dados de cada layer
    struct layer
    {
        float z;                // profundidade da camada
        unsigned int tid;       // texture id OpenGL
        const char* filename;   // caminho do arquivo de imagem
        float offsetx, offsety; // offsets de textura para animação
        float ratex, ratey;     // taxa de deslocamento (parallax)
    };

    std::vector<layer *> layers; // lista dinâmica de camadas
    Shader shader_programme;     // shader usado para desenhar as layers
    GLuint VAO_LAYER, VBO, EBO;  // objetos OpenGL para o quad das layers
    float *vertices;             // ponteiro para os dados de vértice (pos + uv)
    float PARALLAX_RATE;         // taxa global aplicada às camadas

    // Carrega uma textura a partir do arquivo (usa stb_image)
    void loadTexture(unsigned int &texture, const char* filename);
    // Cria o VAO/VBO/EBO para as layers usando os dados fornecidos
    void criar_vao_layer(float *vertices, int size_vertices, unsigned int *indices, int size_indices);
};