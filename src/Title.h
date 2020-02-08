#pragma once
#ifndef __Title__
#define __Title__

#include "GameObject.h"
#include "TextureManager.h"

class Title : public GameObject {
public:
	Title();
	~Title();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
private:
};


#endif /* defined (__Title__) */