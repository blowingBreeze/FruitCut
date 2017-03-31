#include "parabola.h"


ParabolaAction::ParabolaAction()
{
}


ParabolaAction::~ParabolaAction()
{
}

ParabolaAction* ParabolaAction::create(Point startPoint, Velocity startVelocity,float gravity,float duration)
{
	ParabolaAction* pRet = new ParabolaAction();
	pRet->init(startPoint, startVelocity,gravity,duration);
	pRet->autorelease();

	return pRet;
}

bool ParabolaAction::init(Point startPoint, Velocity startVelocity,float gravity,float duration)
{
	this->m_gravity = gravity;
	this->m_startPoint = startPoint;
	this->m_startVelocity = startVelocity;
	this->m_duration = duration;
	if (!ActionInterval::initWithDuration(m_duration))
		return false;

	return true;
}

void ParabolaAction::startWithTarget(Node* pTarget)
{
	this->m_pTarget = pTarget;
	ActionInterval::startWithTarget(pTarget);
}

void ParabolaAction::update(float t)
{
	if (!m_pTarget)
		return;

	auto elapsed = t*m_duration; //current time
	currentX = m_startPoint.x + m_startVelocity.x*elapsed;
	currentY =m_startPoint.y+m_startVelocity.y*elapsed - 0.5*m_gravity*elapsed*elapsed;
	m_pTarget->setPosition(Vec2(currentX, currentY));
}