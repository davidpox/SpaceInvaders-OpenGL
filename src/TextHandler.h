#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "bullet.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TextHandler
{
public:
	TextHandler();
	~TextHandler();

	GLuint createSprite(std::string text, int fontsize);
	GLuint createShaderProgram();

	GLuint VAO, VBO, EBO, texture;
	glm::mat4 _transRotate, _transTranslate, _transScale;
	float h, w;

};

