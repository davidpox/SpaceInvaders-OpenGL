#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class bullet
{
public:
	bullet(std::string name);
	~bullet();

	GLuint createSprite(std::string pic);
	GLuint createShaderProgram();
	void shootBullet();
	void arrangeToAlien();
	bool isActive;
	bool moved;
	void resetPositionX(glm::vec3 playerPos, SDL_Keycode dir);
	void resetPositionY();
	void resetPositionAL();

	glm::vec3 position = {0.0f, -0.775f, 0.0f};
	glm::vec3 distTravelled = { 0.0f, 0.0f, 0.0f };
	glm::vec3 alTravel = { 0.0f, 0.0f, 0.0f }; 
	glm::mat4 _transRotate, _transTranslate, _transScale;

	float h;
	float w;

	std::string name;



	SDL_Surface* img;
	GLuint VAO, VBO, EBO, texture;
};

