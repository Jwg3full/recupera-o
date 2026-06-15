#include "Layer.h"

Layer::Layer(float *vertices, int size_vertices, unsigned int *indices, int size_indices)
{

	// Inicializa VAO/VBO/EBO para desenhar o quad que será usado por cada layer
	Layer::criar_vao_layer(vertices, size_vertices, indices, size_indices);
	// Taxa global de parallax começa em zero (sem movimento)
	PARALLAX_RATE = 0.0f;
	Shader shader_programme; // construtor local (não necessário, mas explícito)
}

void Layer::adicionar_layer(float z, float ratex,const char* filename)
{

	// Cria dinamicamente uma nova estrutura layer e inicializa seus campos
	layer *l = new layer;
	l->filename = filename;
	l->z = z;
	l->offsetx = 0;
	l->offsety = 0;
	l->ratex = ratex;
	l->ratey = 0;
	layers.push_back(l);
	// Carrega textura da imagem para um texture id OpenGL
	loadTexture(l->tid, l->filename);
}

void Layer::criar_vao_layer(float *vertices, int size_vertices, unsigned int *indices, int size_indices)
{

	glGenVertexArrays(1, &VAO_LAYER);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO_LAYER);

	// Preenche o VBO com os dados de posição+UV
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_STATIC_DRAW);

	// Preenche o EBO com os índices para desenhar dois triângulos
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, indices, GL_STATIC_DRAW);

	// Attrib 0 = posição (x,y)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Attrib 1 = coordenadas de textura (u,v)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Layer::carregar_layer()
{	

	// Usa o programa de shader das layers
	glUseProgram(shader_programme.get_program());
	glBindVertexArray(VAO_LAYER);
	// Para cada layer, atualiza offsets e uniformes, vincula textura e desenha o quad
	for (int i = 0; i < layers.size(); i++)
	{
		// Atualiza o deslocamento horizontal com base na taxa e no PARALLAX_RATE global
		layers[i]->offsetx += layers[i]->ratex * PARALLAX_RATE;

		// Envia uniforms ao shader: offsets, profundidade e sampler
		glUniform1f(glGetUniformLocation(shader_programme.get_program(), "offsetx"), layers[i]->offsetx);
		glUniform1f(glGetUniformLocation(shader_programme.get_program(), "offsety"), layers[i]->offsety);
		glUniform1f(glGetUniformLocation(shader_programme.get_program(), "layer_z"), layers[i]->z);
		// Vincula a textura da layer ao sampler 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, layers[i]->tid);
		glUniform1i(glGetUniformLocation(shader_programme.get_program(), "sprite"), 0);
		// Desenha os 6 índices (2 triângulos)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void Layer::mover_direita()
{

	// Ao mover para direita, as layers se deslocam com taxa positiva
	PARALLAX_RATE = 0.003f;
}

void Layer::mover_esquerda()
{

	// Ao mover para esquerda, as layers se deslocam com taxa negativa
	PARALLAX_RATE = -0.003f;
}

void Layer::parado()
{

	// Para qualquer deslocamento
	PARALLAX_RATE = 0;
}

void Layer::criar_program_shader()
{	
	bool status;
	// Carrega os fontes dos shaders e compila/linka o programa
	printf("Carregando arquivos \n");
	shader_programme.carregar_arquivo("_camadas_vs.glsl","_camadas_fs.glsl");

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

void Layer::loadTexture(unsigned int &texture, const char *filename)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	int width, height, nrChannels;

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		// Determina se a imagem possui canal alpha e escolhe o formato correto
		if (nrChannels == 4)
		{
			cout << "Alpha channel" << endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			cout << "Without Alpha channel" << endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		// Gera mipmaps para melhorar qualidade em diferentes escalas
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
