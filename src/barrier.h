#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include "bullet.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class barrier
{
public:
	barrier();
	~barrier();
	int barrierCount;
	glm::mat4 _transTranslate, _transRotate, _transScale;
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };


	void breakBarrier();
	void arrange(float xOffset, float yOffset);
	GLuint createSprite(std::string pic);
	GLuint createShaderProgram();
};

