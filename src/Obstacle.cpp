#include "Obstacle.h"
#include "Game.h"

Obstacle::Obstacle()
{
	TheTextureManager::Instance()->load("../Assets/textures/Obstacle.png",
		"obstacle", TheGame::Instance()->getRenderer());
	TheSoundManager::Instance()->load("../Assets/audio/thunder.ogg",
		"thunder", sound_type::SOUND_SFX);

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(700.0f, 300.0f));
	setVelocity(glm::vec2(0, 0));
	setIsColliding(false);
	setType(GameObjectType::OBSTACLE);
}

Obstacle::~Obstacle()
{
}

void Obstacle::draw()
{

	int xComponent = getPosition().x;
	int yComponent = getPosition().y;
	TheTextureManager::Instance()->draw("obstacle", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), true);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
