#include "barrier.h"

barrier::barrier()
{
	h = 0.1f;
	w = 0.1f;
	barrierIndex = 0;
}


barrier::~barrier()
{
}

GLuint barrier::createSprite(std::string pic) {
	GLfloat verts[] = {
		// Positions							 // Texture Coords 
		-0.8f, -0.6f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // Bottom Left
		-0.7f, -0.6f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // Bottom Right
		-0.7f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // Top Right
		-0.8f, -0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f    // Top Left 
	};
	GLuint ind[] = {
		0, 1, 3,
		1, 2, 3
	};

	std::string filename = "bin/assets/" + pic + ".png";
	img = IMG_Load(filename.c_str());
	//std::cout << "loaded::" + filename << std::endl;
	if (img == NULL) {
		std::cout << "BARRIER IMAGE LOAD:: " << IMG_GetError() << std::endl;
		return (GLuint)1;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(img);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

	// Pos 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	return VAO;
}

GLuint barrier::createShaderProgram() {
	GLuint shaderProgram, vertexShader, fragmentShader;

	const GLchar* vertexShaderSource[] = {
		"#version 440 core															\n"
		"layout (location = 0) in vec3 position;									\n"
		"layout (location = 1) in vec3 color;										\n"
		"layout (location = 2) in vec2 texCoord;									\n"
		"out vec2 TexCoord;															\n"
		"out vec3 ourColor;															\n"
		"uniform mat4 trans;														\n"
		"void main()																\n"
		"{																			\n"
		"gl_Position = trans * vec4(position.x, position.y, position.z, 1.0f);		\n"
		"ourColor = color;															\n"
		"TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);}							\n"
	};
	const GLchar* fragmentShaderSource[] = {
		"#version 440 core															\n"
		"in vec3 ourColor;															\n"
		"in vec2 TexCoord;															\n"
		"uniform sampler2D outTexture;												\n"
		"out vec4 color;															\n"
		"void main()																\n"
		"{																			\n"
		"color = texture(outTexture, TexCoord) * vec4(0.0f, 1.0f, 0.0f, 1.0f);		\n"
		"}																			\n\0"
	};

	vertexShader = glCreateShader(GL_VERTEX_SHADER);		//create vertex shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	//create fragment shader

	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);	//assign GLChar source to shader
	glCompileShader(vertexShader);								//compile shader

	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checking to see if shaders created successfully
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "BARRIER::VERTEX::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "BARRIER::FRAGMENT::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	/* SHADERPROGRAM CREATION */


	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "SHADER::PROGRAM CREATION ERROR: " << infoLog << std::endl;
	}
	if (success) {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	return shaderProgram;
}

void barrier::breakBarrier() {
	std::string filename = "bin/assets/broken_" + std::to_string(barrierIndex) + ".png";
	img = IMG_Load(filename.c_str());
	if (img == NULL) {
		std::cout << "BARRIER IMAGE LOAD:: " << IMG_GetError() << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(img);
}

void barrier::arrange(float xOffset, float yOffset) {
	_transTranslate = glm::translate(_transTranslate, glm::vec3(xOffset, yOffset, 0.0f));
	position.x += xOffset;
	position.y += yOffset;
}