#include "alien.h"

alien::alien()
{
	h = 0.07f;
	w = 0.1f;
	spinofftimer = 0;
	isDead = false;
	currentAnimState = 1;
}


alien::~alien()
{
}

GLuint alien::createSprite(std::string pic) {
	GLfloat verts[] = {
		// Positions			// Color Coords		// Texture Coords 
		-0.95f, 0.88f, 0.0f,	 0.0f, 0.0f, 1.0f,	 0.0f, 0.0f,   // Bottom Left
		-0.85f, 0.88f, 0.0f,	 0.0f, 1.0f, 0.0f,	 1.0f, 0.0f,   // Bottom Right
		-0.85f, 0.95f, 0.0f,	 1.0f, 0.0f, 0.0f,	 1.0f, 1.0f,   // Top Right
		-0.95f, 0.95f, 0.0f,	 1.0f, 1.0f, 0.0f,	 0.0f, 1.0f    // Top Left 
	};
	GLuint ind[] = {
		0, 1, 3,
		1, 2, 3
	};

	std::string filename = "bin/assets/" + pic + ".png";
	img = IMG_Load(filename.c_str());
	if (img == NULL) {
		std::cout << "ALIEN IMAGE LOAD:: " << IMG_GetError() << std::endl;
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

	glBindVertexArray(0);

	return VAO;
}

GLuint alien::createShaderProgram() {
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
		"color = texture(outTexture, TexCoord) * vec4(1.0f, 1.0f, 1.0f, 1.0f);		\n"
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
		std::cout << "ALIEN::VERTEX::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ALIEN::FRAGMENT::SHADER CREATION ERROR: " << infoLog << std::endl;
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

void alien::moveAlien(SDL_Keycode dir) {
	if (position[0] >= -0.9f) {			//&& position[0] >= -0.9f
		if (dir == SDLK_LEFT) {
			_transTranslate = glm::translate(_transTranslate, glm::vec3(-0.05f, 0.0f, 0.0f));
			position[0] -= 0.05f;
		}
	}
	if (position[0] <= 0.9f) {
		if (dir == SDLK_RIGHT) {
			_transTranslate = glm::translate(_transTranslate, glm::vec3(0.05f, 0.0f, 0.0f));
			position[0] += 0.05f;
		}
	}
}

void alien::arrange(float xOffset, float yOffset) {
	_transTranslate = glm::translate(_transTranslate, glm::vec3(xOffset, yOffset, 0.0f));
	position[0] += xOffset;
	position[1] += yOffset;
	posFromStart.x += xOffset;
	posFromStart.y += yOffset;
}


void alien::changeTexture() {
	std::string filename = "bin/assets/alien" + std::to_string(currentAnimState) + ".png";
	img = IMG_Load(filename.c_str());
	if (img == NULL) {
		std::cout << "ALIEN ANIM IMAGE LOAD:: " << IMG_GetError() << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(img);
}