#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
public:
	camera();
	~camera();
	GLuint createShaderProgram();
	void moveCam(SDL_Keycode dir, glm::vec3 position);
	glm::mat4 _modelMat, _viewMat, _projectionMat;
};

