#pragma once

#include <iostream>
#include "alien.h"
#include <vector>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include <random>

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

class shotHandler {
public: 
	shotHandler();
	~shotHandler();
	int getAlien(std::vector<alien> al_vec);
};
