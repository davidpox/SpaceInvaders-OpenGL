#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Background
{
public:
	Background(float fspeed);
	~Background();

	GLuint createSprite(std::string pic);
	GLuint createShaderProgram();
	void moveBG(SDL_Keycode dir);


	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 distTravelled = { 0.0f, 0.0f, 0.0f };
	SDL_Surface* img;
	GLuint VAO, VBO, EBO, texture;
	glm::mat4 _transRotate, _transTranslate, _transScale;

	int h, w;
	float speed;
};


