#include "Game.h"
#include "Title.h"

Title::Title()
{
	TheTextureManager::Instance()->load("../Assets/textures/welcometospace.png",
		"title", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("title");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(400.0f, 300.0f));
	setVelocity(glm::vec2(0, 0));
	setIsColliding(false);
	//setType(GameObjectType::TITLE);

}

Title::~Title()
{
}

void Title::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;
	TheTextureManager::Instance()->draw("title", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), true);
}

void Title::update()
{

}

void Title::clean()
{
}
