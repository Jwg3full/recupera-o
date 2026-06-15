#include "Shader.h"

Shader::Shader()
{

}

bool Shader::carregar_vshader()
{

	// Cria e compila o vertex shader a partir do código em vertex_shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// Verifica se houve erro de compilação
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: GL vertex shader index %i did not compile\n", vs);
		return true; // retorna true indicando erro
	}

	return false;
}

bool Shader::carregar_fshader()
{

	// Cria e compila o fragment shader a partir do código em fragment_shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar *p = (const GLchar *)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// Verifica se houve erro de compilação
	int params = -1;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: GL fragment shader index %i did not compile\n", fs);
		return true;
	}

	return false;
}

bool Shader::carregar_program()
{

	// Cria o programa, anexa shaders e linka
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, vs);
	glAttachShader(shader_programme, fs);
	glLinkProgram(shader_programme);

	// Verifica se link foi bem-sucedido
	int params = -1;
	glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params)
	{
		// Se houver erro, recupera o log para diagnóstico
		int max_length = 2048;
		int actual_length = 0;
		char log[2048];
		glGetProgramInfoLog(shader_programme, max_length, &actual_length, log);
		fprintf(stderr, "ERRO NO LINK DO PROGRAMA (Index %i):\n%s\n", shader_programme, log);
		return true; 
	}

	// Obtém a location do uniform usado para passar a matriz de transformação
	GLtransform = glGetUniformLocation(shader_programme, "transformacao");
	return false;
}

GLuint Shader::get_GLtrasform()
{
    return GLtransform;
}

GLint Shader::get_program(){

	return shader_programme;
}

bool Shader::carregar_arquivo(const char* vs, const char* fs)
{
    gl_utils::parse_file_into_str(vs, vertex_shader, 1024 * 256);
	gl_utils::parse_file_into_str(fs, fragment_shader, 1024 * 256);

	// Retorna falso (0) para indicar sucesso na leitura
	return 0;
}
