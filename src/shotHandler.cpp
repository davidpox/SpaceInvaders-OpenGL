#include "shotHandler.h"


shotHandler::shotHandler() {

}
shotHandler::~shotHandler() {

}

int shotHandler::getAlien(std::vector<alien> al_vec) {
	int first, last;

	if (al_vec.size() > 10) {
		first = (int)al_vec.size() - 11;
		last = (int)al_vec.size();
		return glm::linearRand(first, last) - 1;
	}
	else {
		return glm::linearRand(0, (int)al_vec.size()) - 1;
	}
	
	return -1;
}
