#include "Triangulo.h"

Triangulo::Triangulo(){ // Construtor da classe Triangulo
    // Inicializa posição e profundidade
    posicao.x = 0.0f;
    posicao.y = 0.0f;
    posZ = 0.0f; ////novo🐵
    Shader shader_programme; // invoca construtor do shader 
    
}

void Triangulo::criar_triangulo(float* points, size_t tamanhoTotal){
    // Cria VBO e armazena os pontos (cada vértice tem x,y,z)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, tamanhoTotal, points, GL_STATIC_DRAW);

    // Cria VAO e descreve o layout: atributo 0 = posição (vec3)
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

}

void Triangulo::carregar_triangulo()
{

    //  shader
    glUseProgram(shader_programme.get_program());

    //  Calcula a matriz de transformação (model matrix) e envia para o shader
    mover_triangulo();
    
    //  Desenha o triângulo usando o VAO e o shader
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

}

void Triangulo::criar_program_shader()
{

    bool status;

    // Carrega, compila e linka os shaders para sprites
    printf("Carregando arquivos \n");
    shader_programme.carregar_arquivo("_sprites_vs.glsl","_sprites_fs.glsl");

    printf("Carregando vshader \n");
    status = shader_programme.carregar_vshader();
    if (status == true)
    {
        printf("erro vshader \n");
    }

    printf("Carregando fshader \n");
    status = shader_programme.carregar_fshader();
    if (status == true)
    {
        printf("erro fshader \n");
    }

    printf("Carregando program \n");
    status = shader_programme.carregar_program();
    if (status == true)
    {
        printf("erro program \n");
    }

}

void Triangulo::set_posicaoY(float y)
{

    posicao.y += y;

}

void Triangulo::mover_triangulo()
{
    glUseProgram(shader_programme.get_program());
    //novo🐵
    
    float escala = 1.0f - posZ; 
    if (escala < 0.01f) escala = 0.01f; // Evita que o triângulo desapareça completamente quando estiver muito longe

    glm::mat4 matrizModel = glm::mat4(1.0f); 
    
    // Aplica a translação para a posição atual do triângulo(X,Y) e a profundidade Z (que afeta a escala)
    matrizModel = glm::translate(matrizModel, glm::vec3(posicao.x, posicao.y, posZ)); //fica no meio sem cordenada Z, mas pode ser ajustado para criar o efeito de profundidade
    
    // Aplica a escala, aumentando ou diminuindo o triângulo com base na profundidade Z
    matrizModel = glm::scale(matrizModel, glm::vec3(escala, escala, 1.0f));
    //novo🐵🐵🐵
    glUniformMatrix4fv(shader_programme.get_GLtrasform(), 1, GL_FALSE, glm::value_ptr(matrizModel));
}

void Triangulo::gerar_cor()
{

    color.r = (rand() % 100) / 100.0f;
    color.g = (rand() % 100) / 100.0f;
    color.b = (rand() % 100) / 100.0f;

}
//novo🐵🐵🐵 só getters e setters da posição do triangulo para atualizr xyz
void Triangulo::set_posicao(float x, float y, float z) {
    posicao.x = x;
    posicao.y = y;
    posZ = z;
}

void Triangulo::add_posicaoX(float x) {
    posicao.x += x;
}

void Triangulo::set_posZ(float z) {
    posZ = z;
}

float Triangulo::get_posZ() {
    return posZ;
}
//novo🐵





