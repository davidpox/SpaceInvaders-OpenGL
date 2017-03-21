#include <iostream>
#include <vector>
#include <string>
// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#include <GL/glew.h>

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <windows.h>
#include "PlayerShip.h"
#include "bullet.h"
#include "alien.h"
#include "gamestate.h"
#include "barrier.h"
#include "TextHandler.h"
#include "Background.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shotHandler.h" 

SDL_Window *win = nullptr;
SDL_GLContext mainContext;
PlayerShip *player = new PlayerShip();
bullet *bullets = new bullet("player");
bullet *alienBullet = new bullet("alien");
alien *aliens = new alien();
shotHandler *sh = new shotHandler();
gamestate *gs = new gamestate();


std::vector<GLuint> sprog_arr;
std::vector<GLuint> vao_arr;
std::vector<alien> alien_arr;
std::vector<barrier> barrier_arr;
std::vector<TextHandler *> text_arr;
std::vector<PlayerShip *> lives_arr;
std::vector<Background> bg_arr;

bool isGameRunning;

void checkErrors() {
	GLenum error = glGetError();
	switch (error) {
	case GL_INVALID_ENUM:
		std::cout << "An unacceptable value is specified for an enumerated argument." << std::endl;
		break;
	case GL_INVALID_VALUE:
		std::cout << "A numeric argument is out of range." << std::endl;
		break;
	case GL_INVALID_OPERATION:
		std::cout << "The specified operation is not allowed in the current state." << std::endl;
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "The framebuffer object is not complete." << std::endl;
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "There is not enough memory left to execute the command." << std::endl;
		break;
	case GL_STACK_UNDERFLOW:
		std::cout << "An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl;
		break;
	case GL_STACK_OVERFLOW:
		std::cout << "An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl;
		break;
	case GL_NO_ERROR:
	default:
		break;
	}
}

void SetOpenGLAttr() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

static void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR" << std::endl;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR" << std::endl;
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR" << std::endl;
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY" << std::endl;
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE" << std::endl;
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER" << std::endl;
		break;
	}

	(void)source; (void)type; (void)id;
	(void)severity; (void)length; (void)userParam;
	fprintf(stderr, "%s\n", message);
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		fprintf(stderr, "Aborting...\n");
		abort();
	}

}

int init() {
	int screenWidth, screenHeight;
	int winWidth, winHeight;
	SDL_DisplayMode disp;

	// SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
			glGetError());
		return 1;
	}
	SDL_Log("SDL initialised OK!\n");

	if (SDL_GetCurrentDisplayMode(0, &disp) == 0) {
		screenWidth = disp.w;
		screenHeight = disp.h;
	}
	else {
		std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		return 1;
	}


	// Window Creation
	win = SDL_CreateWindow("David Puetter; CGP2012M; PUE15564059", screenWidth/4, screenHeight/4, screenWidth/2, screenHeight/2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (win == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateWindow init error: %s\n", glGetError());
		return 1;
	}
	SDL_GetWindowSize(win, &winWidth, &winHeight);


	// SDL_Image initialise
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_image init error: %s\n",
			IMG_GetError());
		return 1;
	}
	SDL_Log("SDL_image initialised OK!\n");

	// SDL_ttf initialise
	if (TTF_Init() == -1) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf init error: %s\n",
			glGetError());
		return 1;
	}
	SDL_Log("SDL_ttf initialised OK!\n");


	/* CONTEXT AND GLEW CREATION */

	SetOpenGLAttr();

	mainContext = SDL_GL_CreateContext(win);

	glewExperimental = GL_TRUE;
	glewInit();

	if (winWidth > winHeight) {
		glViewport(winWidth/4, 0, winHeight, winHeight);
	}
	else {
		glViewport(0, winHeight/4, winWidth, winWidth);
	}

	//glViewport(0, 0, 500, 500);

	/* DEBUG INITIALISATION*/

	std::cout << "Registering OpenGL Debug callback function" << std::endl;
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);

	/* Shader shit was here */

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glOrtho(0, 0, 500, 500, -1, 1);

return 0;
}

void resizeWindow() {
	int w, h;
	int tw, th;
	SDL_GetWindowSize(win, &w, &h);

	if (w >= h) {
		tw = h;
		th = h;
	}
	if (h > w) {
		tw = w;
		th = w;
	}

	glViewport(0, 0, tw, th);
}

void getInput() {
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0) {
		if (ev.type == SDL_KEYDOWN) {
			switch (ev.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_RIGHT:
				player->movePlayer(ev.key.keysym.sym);
				bg_arr[0].moveBG(ev.key.keysym.sym);
				bg_arr[1].moveBG(ev.key.keysym.sym);
				bullets->resetPositionX(player->position, ev.key.keysym.sym);
				break;
			case SDLK_SPACE:
				bullets->shootBullet();
				break;
			case SDLK_k:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_BLEND);
				break;
			case SDLK_l:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_BLEND);
				break;
			case SDLK_F10:
				SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
				break;
			case SDLK_F11:
				SDL_SetWindowFullscreen(win, 0);
				break;
			default:
				break;
			}
		}
		if (ev.type == SDL_WINDOWEVENT) {
			if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
				resizeWindow();
			}
		}
		if (ev.type == SDL_QUIT) {
			gs->isGameRunning = false;
		}
	}
}

void initAliens() {
	for (int i = 0; i < 55; i++) {
		alien_arr.push_back(alien());
	}
}

void updateScore() {
	std::string text = "SCORE: " + std::to_string(gs->playerscore);
	vao_arr[73] = text_arr[0]->createSprite(text, 24);
}

void moveAliens() {
	if (!gs->isMovingLeft) {
		if (gs->alienMoveCounter < 3) {
			for (int i = 0; i < alien_arr.size(); i++) {
				if (!alien_arr[i].isDead) {
					alien_arr[i]._transTranslate = glm::translate(alien_arr[i]._transTranslate, glm::vec3(0.1f, 0.0f, 0.0f));
					alien_arr[i].position.x += 0.1f;
				}
			}
			gs->alienMoveCounter++;
		}
		else if (gs->alienMoveCounter == 3) {
			for (int i = 0; i < alien_arr.size(); i++) {
				if (!alien_arr[i].isDead) {
					alien_arr[i]._transTranslate = glm::translate(alien_arr[i]._transTranslate, glm::vec3(0.0f, -0.1f, 0.0f));
					alien_arr[i].position.y -= 0.1f;
				}
			}
			gs->alienMoveSpeed += 0.15f;
			gs->alienMoveCounter = 0;
			gs->isMovingLeft = true;
		}
	}
	else if (gs->isMovingLeft) {
		if (gs->alienMoveCounter < 3) {
			for (int i = 0; i < alien_arr.size(); i++) {
				if (!alien_arr[i].isDead) {
					alien_arr[i]._transTranslate = glm::translate(alien_arr[i]._transTranslate, glm::vec3(-0.1f, 0.0f, 0.0f));
					alien_arr[i].position.x -= 0.1f;
				}
			}
			gs->alienMoveCounter++;
		}
		else if (gs->alienMoveCounter == 3) {
			for (int i = 0; i < alien_arr.size(); i++) {
				if (!alien_arr[i].isDead) {
					alien_arr[i]._transTranslate = glm::translate(alien_arr[i]._transTranslate, glm::vec3(0.0f, -0.1f, 0.0f));
					alien_arr[i].position.y -= 0.1f;
				}
			}
			gs->alienMoveSpeed += 0.15f;
			gs->alienMoveCounter = 0;
			gs->isMovingLeft = false;
		}
	}
}

void alienShoot() {
	if (!alienBullet->isActive) {
		int it = sh->getAlien(alien_arr);

		if (it != -1) {
			alien alien_t = alien_arr[it];

			alienBullet->position.x = alien_t.position.x - 0.025f;
			alienBullet->position.y = alien_t.position.y;
			alienBullet->_transTranslate = glm::translate(alienBullet->_transTranslate, glm::vec3(alien_t.position.x - 0.025f, alien_t.position.y, 0.0f));
			alienBullet->isActive = true;
		}
		else {
			std::cout << "ERR ON ALIENSHOOT IT" << std::endl;
		}
	}
}

void degradebarrier() {
	if ((int)barrier_arr.size() > 1) {
		int barrier = glm::linearRand(0, (int)barrier_arr.size());

		barrier_arr[barrier].barrierIndex++;
		if (barrier_arr[barrier].barrierIndex == 3) {
			barrier_arr.erase(barrier_arr.begin() + barrier);
		}
		else {
			barrier_arr[barrier].breakBarrier();
		}
	}
}

void update() {
	if (bullets->isActive) {
		bullets->_transTranslate = glm::translate(bullets->_transTranslate, glm::vec3(0.0f, 0.08f, 0.0f));
		bullets->position.y += 0.08f;
		bullets->distTravelled.y += 0.08f;
	}

	if (bullets->position[1] >= 1.0f) {
		bullets->isActive = false;
		bullets->resetPositionY();
		bullets->resetPositionX(player->position, SDLK_UP);
	}

	if (alienBullet->isActive) {
		alienBullet->_transTranslate = glm::translate(alienBullet->_transTranslate, glm::vec3(0.0f, -0.06f, 0.0f));
		alienBullet->position.y -= 0.06f;
		alienBullet->distTravelled.y -= 0.06f;
	}
	if (alienBullet->position.y <= -0.9f) {
		alienBullet->isActive = false;
		alienBullet->resetPositionAL();
	}

	if (gs->alienMoveTimer == (int)(120 / gs->alienMoveSpeed)) {
		moveAliens();
		for (int i = 0; i < alien_arr.size(); i++) {
			if (alien_arr[i].currentAnimState == 2) {
				alien_arr[i].currentAnimState = 1;
				alien_arr[i].changeTexture();
			}
			else if (alien_arr[i].currentAnimState == 1) {
				alien_arr[i].currentAnimState = 2;
				alien_arr[i].changeTexture();
			}
		}

		std::cout << gs->alienMoveSpeed << std::endl;
		gs->alienMoveTimer = 0;
	}
	else {
		gs->alienMoveTimer++;
	}

	if (alien_arr.size() == 0) {
		gs->gameover = true;
	}

	//int rnd = glm::linearRand(1, 3);

	if (gs->alienShootTimer == gs->rndShot * 60) {
		alienShoot();
		gs->alienShootTimer = 0;
		gs->rndShot = glm::linearRand(1, 3);

	}
	gs->alienShootTimer++;
	
	if (gs->barricadeTimer == 300) {
		degradebarrier();
		gs->barricadeTimer = 0;
	}
	gs->barricadeTimer++;










	/* Tried to make Aliens spin off when killed. shit aint working. ill fix one day. */

	/*for (int i = 0; i < alien_arr.size(); i++) {
		if (alien_arr[i].isDead) {
			glm::highp_vec3 start = { -0.875f, 0.915f, 0.0f };

			float xdist = glm::distance(start.x, alien_arr[i].position.x);
			float ydist = glm::distance(start.y, alien_arr[i].position.y);

			alien_arr[i]._transTranslate = glm::translate(alien_arr[i]._transTranslate, glm::vec3{ -xdist, ydist, 0.0f });
	
			alien_arr[i]._transRotate = glm::rotate(alien_arr[i]._transRotate, glm::radians(0.1f), glm::vec3{ 0.0f, 0.0f, 1.0f });

			alien_arr[i]._transTranslate = glm::translate(alien_arr[i]._transTranslate, glm::vec3{ xdist, -ydist, 0.0f });

		

			std::cout << "X : " << xdist << " | Y : " << ydist << std::endl;
		}
	}*/

}

void render() {

	glm::vec4 backgroundColour = glm::vec4(0, 0, 0, 1.0);		// R, G, B, A			(0.431, 0.564, 0.717, 1.0);
	glClearColor(backgroundColour[0], backgroundColour[1], backgroundColour[2], backgroundColour[3]);
	glClear(GL_COLOR_BUFFER_BIT);	//GL_COLOR_BUFFER_BIT

	 
	if (!gs->gameover) {
		glUseProgram(sprog_arr[6]);
		for (int i = 0; i < bg_arr.size(); i++) {
			GLint bgTransLocation = glGetUniformLocation(sprog_arr[6], "trans");
			glUniformMatrix4fv(bgTransLocation, 1, GL_FALSE, glm::value_ptr(bg_arr[i]._transTranslate * bg_arr[i]._transRotate * bg_arr[i]._transScale));
			glBindTexture(GL_TEXTURE_2D, bg_arr[i].texture);
			glBindVertexArray(vao_arr[i + 79]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}



		// Player
		glUseProgram(sprog_arr[0]);
		GLint playertransLocation = glGetUniformLocation(sprog_arr[0], "trans");
		glUniformMatrix4fv(playertransLocation, 1, GL_FALSE, glm::value_ptr(player->_transTranslate * player->_transRotate * player->_transScale));
		glBindTexture(GL_TEXTURE_2D, player->texture);
		glBindVertexArray(vao_arr[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		if (bullets->isActive) {
			glUseProgram(sprog_arr[1]);
			//bullets->_transTranslate = glm::translate(bullets->_transTranslate, glm::vec3(0.0f, 0.0005f, 0.0f));
			GLint bullettransLocation = glGetUniformLocation(sprog_arr[1], "trans");
			glUniformMatrix4fv(bullettransLocation, 1, GL_FALSE, glm::value_ptr(bullets->_transTranslate * bullets->_transRotate * bullets->_transScale));

			glBindTexture(GL_TEXTURE_2D, bullets->texture);
			glBindVertexArray(vao_arr[1]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glUseProgram(sprog_arr[2]);

		for (int i = 0; i < alien_arr.size(); i++) {
			GLint alienstransLocation = glGetUniformLocation(sprog_arr[2], "trans");
			glUniformMatrix4fv(alienstransLocation, 1, GL_FALSE, glm::value_ptr(alien_arr[i]._transTranslate * alien_arr[i]._transRotate * alien_arr[i]._transScale));
			glBindTexture(GL_TEXTURE_2D, alien_arr[i].texture);
			glBindVertexArray(vao_arr[i + 2]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}


		if (alienBullet->isActive) {
			glUseProgram(sprog_arr[3]);
			GLint bullettransLocation = glGetUniformLocation(sprog_arr[3], "trans");
			glUniformMatrix4fv(bullettransLocation, 1, GL_FALSE, glm::value_ptr(alienBullet->_transTranslate * alienBullet->_transRotate * alienBullet->_transScale));
			glBindTexture(GL_TEXTURE_2D, alienBullet->texture);
			glBindVertexArray(vao_arr[57]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glUseProgram(sprog_arr[4]);
		for (int i = 0; i < barrier_arr.size(); i++) {
			GLint barrierTransLoc = glGetUniformLocation(sprog_arr[4], "trans");
			glUniformMatrix4fv(barrierTransLoc, 1, GL_FALSE, glm::value_ptr(barrier_arr[i]._transTranslate * barrier_arr[i]._transRotate * barrier_arr[i]._transScale));
			glBindTexture(GL_TEXTURE_2D, barrier_arr[i].texture);
			glBindVertexArray(vao_arr[i + 58]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glUseProgram(sprog_arr[5]);
		for (int i = 0; i < text_arr.size() - 1; i++) {
			GLint textTransLoc = glGetUniformLocation(sprog_arr[5], "trans");

			glUniformMatrix4fv(textTransLoc, 1, GL_FALSE, glm::value_ptr(text_arr[i]->_transTranslate * text_arr[i]->_transRotate * text_arr[i]->_transScale));
			glBindTexture(GL_TEXTURE_2D, text_arr[i]->texture);
			glBindVertexArray(vao_arr[i + 73]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		if (gs->playerlives >= 1) {
			glUseProgram(sprog_arr[0]);
			for (int i = 0; i < lives_arr.size(); i++) {
				GLint textTransLoc = glGetUniformLocation(sprog_arr[0], "trans");
				glUniformMatrix4fv(textTransLoc, 1, GL_FALSE, glm::value_ptr(lives_arr[i]->_transTranslate * lives_arr[i]->_transRotate * lives_arr[i]->_transScale));
				glBindTexture(GL_TEXTURE_2D, lives_arr[i]->texture);
				glBindVertexArray(vao_arr[i + 75]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
	}
	else if (gs->gameover) {
		glUseProgram(sprog_arr[5]);
		GLint textTransLoc = glGetUniformLocation(sprog_arr[5], "trans");
		glUniformMatrix4fv(textTransLoc, 1, GL_FALSE, glm::value_ptr(text_arr[2]->_transTranslate * text_arr[2]->_transRotate * text_arr[2]->_transScale));
		glBindTexture(GL_TEXTURE_2D, text_arr[2]->texture);
		glBindVertexArray(vao_arr[78]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	SDL_GL_SwapWindow(win);
}

template <typename A, typename B>
bool checkCollisions(A a, B b) {
	float left_src, left_dst;
	float right_src, right_dst;
	float top_src, top_dst;
	float bottom_src, bottom_dst;


	left_src = a.position.x;
	right_src = a.position.x + a.h;
	top_src = a.position.y;
	bottom_src = a.position.y + a.w;

	left_dst = b.position.x;
	right_dst = b.position.x + b.h;
	top_dst = b.position.y;
	bottom_dst = b.position.y + b.w;

	if (bottom_src <= top_dst) {
		return false;
	}
	if (top_src >= bottom_dst) {
		return false;
	}
	if (right_src <= left_dst) {
		return false;
	}
	if (left_src >= right_dst) {
		return false;
	}
	return true;
}

void setupAliens() {
	float xOffset = 0.0f;	// used to initialise positions
	float yOffset = 0.0f;

	/* Fill alien vector with aliens */
	for (int i = 0; i < 55; i++) {
		alien_arr.push_back(alien());
	}
	/* Then create VAOs for each alien. */
	for (int i = 0; i < alien_arr.size(); i++) {
		vao_arr.push_back(alien_arr[i].createSprite("alien1"));
	}

	for (int i = 0; i < alien_arr.size(); i++) {
		if (i % 11 == 0 && i != 0) {
			yOffset -= 0.15f;
			xOffset = 0.0f;
		}

		alien_arr[i].arrange(xOffset, yOffset);

		xOffset += 0.15f;
	}
}
void setupBarriers(float xOffset, int it) {

	for (int i = 0; i < 5; i++) {
		barrier_arr.push_back(barrier());
	}

	for (int i = 0; i < 5; i++) {
		vao_arr.push_back(barrier_arr[i + (it * 5)].createSprite("t_" + std::to_string(i + 1)));
	}

	for (int i = 0; i < 5; i++) {
		switch (i) {
		case 0:
			barrier_arr[i + (it * 5)].arrange(0.0f + xOffset, 0.0f);
			break;
		case 1:
			barrier_arr[i + (it * 5)].arrange(0.1f + xOffset, 0.0f);
			break;
		case 2:
			barrier_arr[i + (it * 5)].arrange(0.2f + xOffset, 0.0f);
			break;
		case 3:
			barrier_arr[i + (it * 5)].arrange(0.0f + xOffset, -0.1f);
			break;
		case 4:
			barrier_arr[i + (it * 5)].arrange(0.2f + xOffset, -0.1f);
			break;
		default:
			std::cout << "Switch error: " << i <<  std::endl;
			break;
		}
	}
}

void setupEntities() {
	text_arr.push_back(new TextHandler());
	text_arr.push_back(new TextHandler());
	text_arr.push_back(new TextHandler());

	lives_arr.push_back(new PlayerShip());
	lives_arr.push_back(new PlayerShip());
	lives_arr.push_back(new PlayerShip());

	bg_arr.push_back(Background(0.005f));
	bg_arr.push_back(Background(0.01f));


	vao_arr.push_back(player->createSprite("player"));
	vao_arr.push_back(bullets->createSprite("bullet"));
	setupAliens();
	vao_arr.push_back(alienBullet->createSprite("bullet2"));
	setupBarriers(0.0f, 0);
	setupBarriers(0.65f, 1);
	setupBarriers(1.3f, 2);
	vao_arr.push_back(text_arr[0]->createSprite("SCORE: 0", 24));
	vao_arr.push_back(text_arr[1]->createSprite("LIVES: ", 24));
	vao_arr.push_back(lives_arr[0]->createSprite("player"));
	vao_arr.push_back(lives_arr[1]->createSprite("player"));
	vao_arr.push_back(lives_arr[2]->createSprite("player"));
	vao_arr.push_back(text_arr[2]->createSprite("GAME OVER!", 48));
	vao_arr.push_back(bg_arr[0].createSprite("background_bottom"));
	vao_arr.push_back(bg_arr[1].createSprite("background_top"));

	sprog_arr.push_back(player->createShaderProgram());
	sprog_arr.push_back(bullets->createShaderProgram());
	sprog_arr.push_back(alien_arr[0].createShaderProgram());
	sprog_arr.push_back(alienBullet->createShaderProgram());
	sprog_arr.push_back(barrier_arr[0].createShaderProgram());
	sprog_arr.push_back(text_arr[0]->createShaderProgram());
	sprog_arr.push_back(bg_arr[0].createShaderProgram());


	alienBullet->arrangeToAlien();
	text_arr[1]->_transTranslate = glm::translate(text_arr[1]->_transTranslate, glm::vec3(1.0f, 0.0f, 0.0f));
	text_arr[2]->_transTranslate = glm::translate(text_arr[2]->_transTranslate, glm::vec3(1.0f, 0.9f, 0.0f));

	lives_arr[0]->_transTranslate = glm::translate(lives_arr[0]->_transTranslate, glm::vec3(0.5f, -0.175f, 0.0f));
	lives_arr[1]->_transTranslate = glm::translate(lives_arr[1]->_transTranslate, glm::vec3(0.65f, -0.175f, 0.0f));
	lives_arr[2]->_transTranslate = glm::translate(lives_arr[2]->_transTranslate, glm::vec3(0.8f, -0.175f, 0.0f));
}

int main(int argc, char *argv[]) {

	gs->isGameRunning = true;
	float frametime = 0.0f;
	int prevtime = 0;
	int currenttime = 0;
	float deltatime = 0.0f;

	if (init() == 1) {
		std::cout << "Could not init" << std::endl;
		system("PAUSE");
		return 1;
	}

	setupEntities();
	gs->rndShot = glm::linearRand(1, 3);

	std::cout << "OPENGL Version: " << glGetString(GL_VERSION) << std::endl;
	while (gs->isGameRunning) {
		prevtime = currenttime;
		currenttime = SDL_GetTicks();
		deltatime = (currenttime - prevtime) / 1000.0f;
		frametime += deltatime;

		getInput();

		//alienShoot();
		if (frametime >= 0.0166f) {
			update();

			frametime = 0.0f;
		}

		for (int i = 0; i < alien_arr.size(); i++) {						/* Alien <-> PlayerBullet collision */
			if (checkCollisions(alien_arr[i], *bullets)) {
				alien_arr.erase(alien_arr.begin() + i);
				bullets->isActive = false;
				bullets->resetPositionY();
				bullets->resetPositionX(player->position, SDLK_UP);
				gs->playerscore += 100;
				updateScore();
			}
		}

		for (int i = 0; i < barrier_arr.size(); i++) {						/* Barrier <-> PlayerBullet collision */
			if (checkCollisions(barrier_arr[i], *bullets)) {
				barrier_arr[i].barrierIndex++;
				if (barrier_arr[i].barrierIndex == 3) {
					barrier_arr.erase(barrier_arr.begin() + i);
				}
				else {
					barrier_arr[i].breakBarrier();//"broken_" + barrier_arr[i].barrierIndex);
					bullets->isActive = false;
					bullets->resetPositionY();
					bullets->resetPositionX(player->position, SDLK_UP);
				}
			}
		}
		for (int i = 0; i < barrier_arr.size(); i++) {
			if (checkCollisions(barrier_arr[i], *alienBullet)) {
				barrier_arr[i].barrierIndex++;
				if (barrier_arr[i].barrierIndex == 3) {
					barrier_arr.erase(barrier_arr.begin() + i);
				}
				else {
					barrier_arr[i].breakBarrier();
					alienBullet->isActive = false;
					alienBullet->resetPositionAL();
				}
			}
		}
		if (checkCollisions(*alienBullet, *player)) {
			gs->playerlives--;
			if (gs->playerlives > 0) {
				lives_arr.pop_back();
			}

			alienBullet->isActive = false;
			alienBullet->resetPositionAL();
			if (gs->playerlives == 0) {
				gs->gameover = true;
			}
		}
		for (int i = 0; i < alien_arr.size(); i++) {					/* Alien <-> Barrier Detecction */
			for (int j = 0; j < barrier_arr.size(); j++) {
				if (checkCollisions(alien_arr[i], barrier_arr[j])) {
					gs->gameover = true;
				}
			}
		}


		render();
	}

	// Clean up

	delete player;

	SDL_Log("Finished. Cleaning up and closing down\n");

	SDL_Quit();
	return 0;
}