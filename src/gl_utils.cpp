#include "gl_utils.h"



bool gl_utils::criar_janela(){

	// Inicializa GLFW
	if (!glfwInit()){
		std::cout << "Erro para iniciar o GLFW";
		return false;
	}

	// Solicita um contexto OpenGL moderno (3.3 core)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Cria a janela principal
	janela = glfwCreateWindow(800, 600, "Exercicio layer", NULL, NULL);
	if (!janela) {
		std::cout << "Erro para abriar a janela";
		glfwTerminate();
		return false;
	}
	// Torna o contexto atual para chamadas OpenGL
	glfwMakeContextCurrent(janela);

	// Inicializa GLEW para carregar extensões OpenGL
	glewExperimental = GL_TRUE;
	glewInit ();

	// Imprime informações do renderer e versão (útil para debug)
	const GLubyte* renderer = glGetString (GL_RENDERER);
	const GLubyte* version = glGetString (GL_VERSION);
	std::cout << ("renderer: %s\nversion: %s\n", renderer, version);

	return true;
}

bool gl_utils::parse_file_into_str (
	const char* file_name, char* shader_str, int max_len
) {
	shader_str[0] = '\0'; // reset string
	FILE* file = fopen (file_name , "r");
	if (!file) {
        std::cerr << "ERRO: não foi possível abrir o arquivo: " << file_name << std::endl;
		return false;
	}
	int current_len = 0;
	char line[2048];
	strcpy (line, ""); 
	while (!feof (file)) {
		if (NULL != fgets (line, 2048, file)) {
			current_len += strlen (line); 
			if (current_len >= max_len) {
				
			}
			strcat (shader_str, line);
		}
	}
	if (EOF == fclose (file)) {
		 
		std::cout << "Erro";
		return false;
	}
	return true;
}