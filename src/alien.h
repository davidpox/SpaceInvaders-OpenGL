#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include "bullet.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class alien
{
public:
	alien();
	~alien();

	GLuint createSprite(std::string pic);
	GLuint createShaderProgram();
	void moveAlien(SDL_Keycode dir);
	void arrange(float xOffset, float yOffset);

	void changeTexture();
	float timer;
	int spinofftimer;
	bool isDead;
	int currentAnimState;

	glm::vec3 position = { -0.875f, 0.915f, 0.0f };
	glm::vec3 distTravelled = { 0.0f, 0.0f, 0.0f };
	glm::vec3 posFromStart = { 0.0f, 0.0f, 0.0f };
	SDL_Surface* img;
	GLuint VAO, VBO, EBO, texture;
	glm::mat4 _transRotate, _transTranslate, _transScale;
	float h, w;

};

