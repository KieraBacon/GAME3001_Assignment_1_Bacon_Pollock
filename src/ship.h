#pragma once
#ifndef __Ship__
#define __Ship__

#include "GameObject.h"
#include "TextureManager.h"
#include "SoundManager.h"

class ship :
	public GameObject
{
public:
	ship();
	~ship();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	// movement functions
	void turnRight(bool turnToTarget = true);
	void turnLeft(bool turnToTarget = true);
	void move(float mod = 1.0f);

	// getters
	glm::vec2 getTargetPosition();
	glm::vec2 getCurrentDirection();
	float getCurrentHeading();
	float getMaxSpeed();
	float getFeelerAngle();								// ADDED
	float getFeelerLength();							// ADDED
	bool getFeelerCol(short feelerNum);					// ADDED
	float getArrivalTarget();							// ADDED
	float getArrivalRadius();							// ADDED

	// setters
	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setCurSpeed(float newSpeed);					// ADDED
	void setMaxSpeed(float newSpeed);
	void setDistanceToTarget(float distance);			// ADDED
	void setFeelerCol(short feelerNum, bool val);		// ADDED

private:
	
	// steering behaviour functions
	void m_checkSteeringState();
	void m_seek();
	void m_flee();										// ADDED
	void m_checkArrival();
	void m_checkCollisions();
	void m_arrive();									// ADDED
	void m_avoid();										// ADDED

	// ship movement
	void m_checkBounds(bool passthrough = true);		// ADDED PASSTHROUGH CHECK
	void m_reset();
	float m_maxSpeed;
	float m_curSpeed;									// ADDED
	float m_turnRate;
	int m_turnFrame;									// ADDED
	int m_turnFrameMax;									// ADDED
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	void m_changeDirection();
	void m_reorient(bool checkFeelers = true);

	// ship avoidance info
	float m_feelerAngel;								// ADDED
	bool m_feelerCol[3];								// ADDED
	float m_feelerLength;								// ADDED
	glm::vec2 m_leftFeelerPos;							// ADDED
	glm::vec2 m_rightFeelerPos;							// ADDED
	float m_avoidDirection;								// ADDED
	int m_avoidEndFrame;								// ADDED
	int m_avoidEndFrameMax;								// ADDED

	// target info
	float m_angleToTarget;
	float m_distanceToTarget;							// ADDED
	float m_arrivalRadius;								// ADDED
	float m_arrivalTarget;								// ADDED
	glm::vec2 m_targetPosition;
	glm::vec2 m_targetDirection;
	void m_computeTargetDirection();
	void m_computeFleeDirection();						// ADDED
};


#endif /* defined (__Ship__) */

