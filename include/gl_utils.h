#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

// Variáveis globais definidas em main.cpp/gl_utils.cpp
extern GLFWwindow* janela;
extern int alturaJanela;
extern int larguraJanela;

// Utilitários simples para criação de janela e leitura de arquivos de shader
class gl_utils{

    public:

        // Inicializa GLFW/GLEW e cria a janela OpenGL
        static bool criar_janela();
        // Lê um arquivo texto e o copia para um buffer C (usado para shaders)
        static bool parse_file_into_str(const char* file_name, char* shader_str, int max_len);

};