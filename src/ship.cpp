#include "ship.h"
#include "Game.h"
#include "Util.h"
#include "GLM/gtx/rotate_vector.hpp"
#include "SoundManager.h"


ship::ship()
{
	TheTextureManager::Instance()->load("../Assets/textures/ship3.png",
		"ship", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(400.0f, 300.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.1f, 0.0f));				// ADDED VALUE ON X AXIS
	setIsColliding(false);
	setType(GameObjectType::SHIP);
	pushSteeringState(SteeringState::IDLE);
	pushSteeringState(SteeringState::IDLE);
	m_maxSpeed = 3.0f;
	m_curSpeed = 3.0f;
	m_currentHeading = 0.0; // current facing angle
	m_angleToTarget = 0.0f;
	m_feelerAngel = 30.0f;								// ADDED
	m_feelerLength = 150.0f;							// ADDED
	m_distanceToTarget = 0.0f;							// ADDED
	m_arrivalRadius = 175.0f;							// ADDED
	m_arrivalTarget = 45.0f;							// ADDED
	m_turnRate = 5.0f; // 5 degrees per frame
	m_turnFrame = 0;									// ADDED
	m_turnFrameMax = 3;									// ADDED
	m_avoidEndFrame = 0;								// ADDED
	m_avoidEndFrameMax = 21;							// ADDED
	m_currentDirection = glm::vec2(1.0f, 0.0f); // facing right

	// Load music
	TheSoundManager::Instance()->load("../Assets/audio/SpaceSong2.ogg", "SpaceMusic", sound_type::SOUND_MUSIC);
	// Load sfx
	TheSoundManager::Instance()->load("../Assets/audio/leaveScreen.ogg", "LeaveScreen", sound_type::SOUND_SFX);
	TheSoundManager::Instance()->load("../Assets/audio/Arrive.ogg", "ArriveSound", sound_type::SOUND_SFX);

	// Play music
	bool musicPlaying = false;
	if (musicPlaying == false)
	{
		TheSoundManager::Instance()->playMusic("SpaceMusic", -1);
		musicPlaying = true;
	}
}

ship::~ship()
{
}

void ship::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("ship", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
}

void ship::m_checkSteeringState()
{
	switch (getState())
	{
	case SteeringState::IDLE:
		break;
	case SteeringState::SEEK:
		m_curSpeed < m_maxSpeed ? m_curSpeed += getAcceleration().x : m_curSpeed = m_maxSpeed;
		m_seek();
		m_reorient();
		move();
		m_checkBounds();
		if (getState(0) != SEEK && getState(0) != ARRIVE)
		{
			m_checkCollisions();
		}
		m_checkArrival();
		break; 
	case SteeringState::ARRIVE:
		m_seek();
		m_reorient();
		if (getState(0) != SEEK)
		{
			m_arrive();
		}
		move();
		m_checkBounds();
		if (getState(0) != SEEK && getState(0) != ARRIVE)
		{
			m_checkCollisions();
		}
		m_checkArrival();
		break;
	case SteeringState::AVOID:
		m_seek();
		m_checkCollisions();
		m_avoid();
		m_arrive();
		move();
		m_checkBounds();
		m_checkArrival();
		break;
	case SteeringState::FLEE:
		m_curSpeed < m_maxSpeed ? m_curSpeed += getAcceleration().x : m_curSpeed = m_maxSpeed;
		m_flee();
		m_reorient();
		move();
		m_checkBounds(false);
		m_checkCollisions();
		break;
	}
}

void ship::update()
{
	++m_turnFrame;
	m_checkSteeringState();
}

void ship::clean()
{
}

void ship::turnRight(bool turnToTarget)
{
	if (m_turnFrame >= m_turnFrameMax)
	{
		m_turnFrame = 0;
		if (turnToTarget)
		{
			float angleDifference = abs(m_turnRate - m_angleToTarget);
			m_currentHeading += (angleDifference < m_turnRate) ? m_angleToTarget * 0.2 : m_turnRate;
		}
		else
		{
			m_currentHeading += m_turnRate;
		}
		m_changeDirection();
		m_computeTargetDirection();
	}
}

void ship::turnLeft(bool turnToTarget)
{
	if (m_turnFrame >= m_turnFrameMax)
	{
		m_turnFrame = 0;
		if (turnToTarget)
		{
			float angleDifference = abs(m_turnRate - m_angleToTarget);
			m_currentHeading -= (angleDifference < m_turnRate) ? m_angleToTarget * 0.2 : m_turnRate;
		}
		else
		{
			m_currentHeading -= m_turnRate;
		}
		m_changeDirection();
		m_computeTargetDirection();
	}
}

void ship::m_reorient(bool checkFeelers)
{
	if ((checkFeelers == true && getFeelerCol(0) == false && getFeelerCol(1) == false && getFeelerCol(2) == false))
	{
		m_angleToTarget = Util::signedAngle(getCurrentDirection(), m_targetDirection);
		if (m_angleToTarget > 0.0f)
			turnRight();
		else if (m_angleToTarget < 0.0f)
			turnLeft();
	}
}

void ship::move(float mod)
{
	setVelocity(getCurrentDirection());

	glm::vec2 newPosition = getPosition() + getVelocity() * m_curSpeed * mod;
	setPosition(newPosition);
}

glm::vec2 ship::getTargetPosition()
{
	return m_targetPosition;
}

glm::vec2 ship::getCurrentDirection()
{
	return m_currentDirection;
}

float ship::getCurrentHeading()
{
	return m_currentHeading;
}

float ship::getMaxSpeed()
{
	return m_maxSpeed;
}

float ship::getFeelerAngle()
{
	return m_feelerAngel;
}

float ship::getFeelerLength()
{
	return m_feelerLength;
}

bool ship::getFeelerCol(short feelerNum)
{
	return m_feelerCol[feelerNum];
}

float ship::getArrivalTarget()
{
	return m_arrivalTarget;
}

float ship::getArrivalRadius()
{
	return m_arrivalRadius;
}

void ship::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void ship::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void ship::setCurSpeed(float newSpeed)
{
	m_curSpeed = newSpeed;
}

void ship::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void ship::setDistanceToTarget(float distance)
{
	m_distanceToTarget = distance;
}

void ship::setFeelerCol(short feelerNum, bool val)
{
	m_feelerCol[feelerNum] = val;
}

void ship::m_checkBounds(bool passthrough)
{
	if (passthrough == true)
	{
		if (getPosition().x > 800)
		{
			TheSoundManager::Instance()->playSound("LeaveScreen", 0);
			setPosition(glm::vec2(0.0f, getPosition().y));
		}
		if (getPosition().x < 0)
		{
			TheSoundManager::Instance()->playSound("LeaveScreen", 0);
			setPosition(glm::vec2(800.0f, getPosition().y));
		}
		if (getPosition().y > 600)
		{
			TheSoundManager::Instance()->playSound("LeaveScreen", 0);
			setPosition(glm::vec2(getPosition().x, 0.0f));
		}
		if (getPosition().y < 0)
		{
			TheSoundManager::Instance()->playSound("LeaveScreen", 0);
			setPosition(glm::vec2(getPosition().x, 600.0f));
		}
	}
	else
	{
		if ((getPosition().x > 800 - getHeight() * 0.75 && getPosition().y > 600 - getHeight() * 0.75) ||
			(getPosition().x < 0 + getHeight() * 0.75 && getPosition().y > 600 - getHeight() * 0.75) ||
			(getPosition().x > 800 - getHeight() * 0.75 && getPosition().y < 0 + getHeight() * 0.75) ||
			(getPosition().x < 0 + getHeight() * 0.75 && getPosition().y < 0 + getHeight() * 0.75))
		{
			setSteeringState(IDLE);
			setSteeringState(IDLE, 0);
		}

		if (getPosition().x > 800 - getHeight() * 0.5)
		{
			setPosition(glm::vec2(800 - getHeight() * 0.5, getPosition().y));
		}

		if (getPosition().x < 0 + getHeight() * 0.5)
		{
			setPosition(glm::vec2(getHeight() * 0.5, getPosition().y));
		}

		if (getPosition().y > 600 - getHeight() * 0.5)
		{
			setPosition(glm::vec2(getPosition().x, 600 - getHeight() * 0.5));
		}

		if (getPosition().y < 0 + getHeight() * 0.5)
		{
			setPosition(glm::vec2(getPosition().x, 0 + getHeight() * 0.5));
		}
	}
}

void ship::m_reset()
{

}

void ship::m_changeDirection()
{
	setCurrentDirection(Util::convertHeadingToDirection(m_currentHeading));

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("ship");
}

void ship::m_computeTargetDirection()
{
	glm::vec2 steeringVelocity;
	steeringVelocity = getTargetPosition() - getPosition();
	m_targetDirection = Util::normalize(steeringVelocity);
}

void ship::m_computeFleeDirection()
{
	glm::vec2 steeringVelocity;
	glm::vec2 nearCorner = { getPosition().x < getTargetPosition().x ? getWidth() * 0.75 : 800 - getWidth() * 0.75, getPosition().y < getTargetPosition().y ? getWidth() * 0.75 : 600 - getWidth() * 0.75 };
	float tx = (abs(nearCorner.x - getPosition().x) / abs(nearCorner.x - getTargetPosition().x));
	float ty = (abs(nearCorner.y - getPosition().y) / abs(nearCorner.y - getTargetPosition().y));
	steeringVelocity.x = Util::lerp(getPosition().x - getTargetPosition().x, nearCorner.x - getPosition().x, tx < ty ? 1 - tx : 1 - ty);
	steeringVelocity.y = Util::lerp(getPosition().y - getTargetPosition().y, nearCorner.y - getPosition().y, tx < ty ? 1 - tx : 1 - ty);
	m_targetDirection = Util::normalize(steeringVelocity);
}


void ship::m_seek()
{
	m_computeTargetDirection(); 
}

void ship::m_flee()
{
	m_computeFleeDirection();

}

void ship::m_checkArrival()
{
	m_distanceToTarget = Util::distance(getPosition(), getTargetPosition());
	if (m_distanceToTarget <= m_arrivalTarget)
	{
		TheSoundManager::Instance()->playSound("ArriveSound", 0);
		setSteeringState(IDLE);
	}
	else if (m_distanceToTarget <= m_arrivalRadius)
	{
		setSteeringState(ARRIVE);
	}
}

void ship::m_checkCollisions()
{
	//glm::vec2 leftFeeler = getPosition() + Util::convertHeadingToDirection(getCurrentHeading() - getFeelerAngle()) * getFeelerLength();
	//glm::vec2 centreFeeler = getPosition() + Util::convertHeadingToDirection(getCurrentHeading()) * getFeelerLength();
	//glm::vec2 rightFeeler = getPosition() + Util::convertHeadingToDirection(getCurrentHeading() + getFeelerAngle()) * getFeelerLength();

	// Check left feeler
	if (CollisionManager::lineAABBCheck(this, Game::Instance()->getObstacle(), getFeelerAngle() * -1.0f, getFeelerLength()))
	{
		setFeelerCol(0, true);
	}
	else
	{
		setFeelerCol(0, false);
	}

	// Check centre feeler
	if (CollisionManager::lineAABBCheck(this, Game::Instance()->getObstacle()))
	{
		setFeelerCol(1, true);
	}
	else
	{
		setFeelerCol(1, false);
	}


	// Check right feeler
	if (CollisionManager::lineAABBCheck(this, Game::Instance()->getObstacle(), getFeelerAngle(), getFeelerLength()))
	{
		setFeelerCol(2, true);
	}
	else
	{
		setFeelerCol(2, false);
	}

	// Check any feeler
	if (getFeelerCol(0) == true || getFeelerCol(1) == true || getFeelerCol(2) == true)
	{
		setDistanceToTarget(sqrt(CollisionManager::circleAABBsquaredDistance(getPosition(), getHeight(), Game::Instance()->getObstacle()->getPosition(), Game::Instance()->getObstacle()->getWidth() * 0.5, Game::Instance()->getObstacle()->getHeight())));
		setSteeringState(AVOID);
		m_avoidEndFrame = 0;

		if ((getFeelerCol(0) == true && getFeelerCol(1) == false && getFeelerCol(2) == false) ||
			(getFeelerCol(0) == false && getFeelerCol(1) == true && getFeelerCol(2) == false) ||
			(getFeelerCol(0) == true && getFeelerCol(1) == true && getFeelerCol(2) == false) ||
			(getFeelerCol(0) == true && getFeelerCol(1) == false && getFeelerCol(2) == true) ||
			(getFeelerCol(0) == true && getFeelerCol(1) == true && getFeelerCol(2) == true))
		{
			m_avoidDirection = -1.0;
		}
		else if ((getFeelerCol(0) == false && getFeelerCol(1) == false && getFeelerCol(2) == true) ||
			(getFeelerCol(0) == false && getFeelerCol(1) == true && getFeelerCol(2) == true))
		{
			m_avoidDirection = 1.0;
		}
	}
	else
	{
		++m_avoidEndFrame;
		if (m_avoidEndFrame >= m_avoidEndFrameMax)
		{
			switch (getState(0))
			{
			case IDLE:
				setSteeringState(IDLE);
				break;
			case SEEK:
			case ARRIVE:
			case AVOID:
				setSteeringState(SEEK);
				break;
			case FLEE:
				setSteeringState(FLEE);
				break;
			}
		}
	}

	// Check collisions
	if (getState(0) != FLEE && CollisionManager::squaredRadiusCheck(this, Game::Instance()->getTarget()))
	{
		setSteeringState(IDLE);
	}
	if (CollisionManager::circleAABBCheck(this, Game::Instance()->getObstacle()))
	{
		move(-2.0f);
		std::cout << "Pushed back!" << std::endl;
	}
}

void ship::m_arrive()
{
	if (m_distanceToTarget <= m_arrivalRadius)
	{
		m_curSpeed = Util::lerp(0.0f, m_maxSpeed, (m_distanceToTarget / m_arrivalRadius));
	}
	else
	{
		//m_curSpeed = m_maxSpeed;
		m_curSpeed < m_maxSpeed ? m_curSpeed += getAcceleration().x : m_curSpeed = m_maxSpeed;
	}
}

void ship::m_avoid()
{
	if (m_avoidDirection < 0)
	{
		turnRight(false);
	}
	else if (m_avoidDirection > 0)
	{
		turnLeft(false);
	}
	
	/*if (getFeelerCol(0) == true && getFeelerCol(1) == false && getFeelerCol(2) == false)
		turnRight();
	if (getFeelerCol(0) == false && getFeelerCol(1) == true && getFeelerCol(2) == false)
		turnRight();
	if (getFeelerCol(0) == false && getFeelerCol(1) == false && getFeelerCol(2) == true)
		turnLeft();
	if (getFeelerCol(0) == true && getFeelerCol(1) == true && getFeelerCol(2) == false)
		turnRight();
	if (getFeelerCol(0) == false && getFeelerCol(1) == true && getFeelerCol(2) == true)
		turnLeft();
	if (getFeelerCol(0) == true && getFeelerCol(1) == false && getFeelerCol(2) == true)
		turnRight();
	if (getFeelerCol(0) == true && getFeelerCol(1) == true && getFeelerCol(2) == true)
		turnRight();*/

	//if (getFeelerCol(0) == true)
	//{
	//	turnRight();
	//}
	//if (getFeelerCol(1) == true)
	//{
	//	turnRight();
	//}
	//if (getFeelerCol(2) == true)
	//{
	//	turnLeft();
	//}
}
