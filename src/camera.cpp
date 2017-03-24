#include "camera.h"



camera::camera()
{
}


camera::~camera()
{
}

GLuint camera::createShaderProgram() {
	GLuint shaderProgram, vertexShader, fragmentShader;

	const GLchar* vertexShaderSource[] = {
		"#version 440 core															\n"
		"layout (location = 0) in vec3 position;									\n"
		"layout (location = 1) in vec3 color;										\n"
		"layout (location = 2) in vec2 texCoord;									\n"
		"out vec2 TexCoord;															\n"
		"out vec3 ourColor;															\n"
		"uniform mat4 trans;														\n"
		"uniform mat4 modelMat;														\n"
		"uniform mat4 viewMat;														\n"
		"uniform mat4 projectionMat;												\n"
		"void main()																\n"
		"{																			\n"
		"gl_Position = projectionMat * viewMat * vec4(position.x, position.y, position.z, 1.0f); \n"
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
		"color = texture(outTexture, TexCoord) * vec4(1.0f, 1.0f, 1.0f, 1.0f);		\n"		// the vec4 controls the colour. could do a uniform but why 
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
		std::cout << "CAMERA::VERTEX::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "CAMERA::FRAGMENT::SHADER CREATION ERROR: " << infoLog << std::endl;
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

void camera::moveCam(SDL_Keycode dir, glm::vec3 position) {
	if (position[0] <= 0.95f) {			//&& position[0] >= -0.9f
		if (dir == SDLK_RIGHT) {
			 _viewMat = glm::translate(_viewMat, glm::vec3(-0.05f, 0.0f, 0.0f));
			//position[0] -= 0.05f;
		}
	}
	if (position[0] >= -0.95f) {
		if (dir == SDLK_LEFT) {
			_viewMat = glm::translate(_viewMat, glm::vec3(0.05f, 0.0f, 0.0f));
			//position[0] += 0.05f;
		}
	}
}