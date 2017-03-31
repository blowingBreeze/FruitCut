#ifndef PARABOLA_H
#define PARABOLA_H
#include "cocos2d.h"

USING_NS_CC;

typedef Point Velocity;


class ParabolaAction : public ActionInterval
{
public:
	ParabolaAction();
	virtual ~ParabolaAction();

	static ParabolaAction* create(Point startPoint, Velocity startVelocity,float gravity,float duration);
	bool init(Point startPoint, Velocity startVelocity,float gravity,float duration);
	virtual void startWithTarget(Node* pTarget);
	virtual void update(float time);


	void setGravity(float gravity=400)
	{
		this->m_gravity = gravity;
	}

private:
	int m_fruitNum;
	Point m_startPoint;
	Velocity m_startVelocity;
	float m_duration;
	float m_gravity;
	Node* m_pTarget; 
	float currentX;
	float currentY;

};

#endif
