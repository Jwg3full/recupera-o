#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl_utils.h"

// Classe utilitária para compilar e gerenciar shaders GLSL
class Shader
{

public:
    // Construtor padrão
    Shader();
    // Retorna o identificador do programa OpenGL ligado
    GLint get_program();
    // Carrega arquivos de shader (vertex e fragment) para buffers internos
    bool carregar_arquivo(const char* vs, const char* fs);
    // Compila vertex shader carregado
    bool carregar_vshader();
    // Compila fragment shader carregado
    bool carregar_fshader();
    // Linka os shaders compilados em um programa e obtém locations úteis
    bool carregar_program();
    // Retorna a location do uniform de transformação (matriz)
    GLuint get_GLtrasform();

private:
    // Buffers para armazenar o código-fonte dos shaders
    char vertex_shader[1024 * 256];
    char fragment_shader[1024 * 256];
    // IDs OpenGL para vertex shader, fragment shader e programa
    GLuint vs, fs, shader_programme, GLtransform;

};