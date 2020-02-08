#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle();
	~Obstacle();
	void draw() override;
	void update() override;
	void clean() override;
};

#endif /* defined (__OBSTACLE__) */
