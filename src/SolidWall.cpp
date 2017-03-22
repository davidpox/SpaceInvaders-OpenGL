#include "SolidWall.h"



SolidWall::SolidWall()
{
	h = 1.0f;
	w = 0.1f;
}


SolidWall::~SolidWall()
{
}

GLuint SolidWall::createSprite() {
	GLfloat verts[] = {
		// Positions			// Color Coords 
		-0.025f, -1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	// BL
		 0.025f, -1.0f, 0.0f,		0.0f, 1.0f, 0.0f, // BR
		 0.025f,  1.0f, 0.0f,		1.0f, 0.0f, 0.0f, // TR
		-0.025f,  1.0f, 0.0f,		1.0f, 1.0f, 0.0f	// TL
	};
	GLuint ind[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

	// Pos 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	return VAO;
}

GLuint SolidWall::createShaderProgram() {
	GLuint shaderProgram, vertexShader, fragmentShader;

	const GLchar* vertexShaderSource[] = {
		"#version 440 core															\n"
		"layout (location = 0) in vec3 position;									\n"
		"layout (location = 1) in vec3 color;										\n"
		"out vec3 ourColor;															\n"
		"uniform mat4 trans;														\n"
		"void main()																\n"
		"{																			\n"
		"gl_Position = trans * vec4(position.x, position.y, position.z, 1.0f);		\n"
		"ourColor = color;}															\n"
	};
	const GLchar* fragmentShaderSource[] = {
		"#version 440 core															\n"
		"in vec3 ourColor;															\n"
		"out vec4 color;															\n"
		"void main()																\n"
		"{																			\n"
		"color = vec4(1.0f, 1.0f, 1.0f, 1.0f);										\n"
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
		std::cout << "PLAYERSHIP::VERTEX::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "PLAYERSHIP::FRAGMENT::SHADER CREATION ERROR: " << infoLog << std::endl;
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