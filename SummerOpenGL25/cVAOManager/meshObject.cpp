#include "meshObject.h"

meshObject::meshObject() {

	this->meshFileName = "";
	this->modelName = "";
	this->position = glm::vec3(0.0f);
	this->orientation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);
	this->color = glm::vec3(1.0f);

	this->isVisible = true;
	this->isWireFrame = true;
}