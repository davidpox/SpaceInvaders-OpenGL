#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SolidWall
{
public:
	SolidWall();
	~SolidWall();

	GLuint createSprite();
	GLuint createShaderProgram();


	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	GLuint VAO, VBO, EBO;
	glm::mat4 _transRotate, _transTranslate, _transScale;

	int h, w;
};

