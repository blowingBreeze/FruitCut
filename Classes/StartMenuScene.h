#ifndef STARTMENUSCENE_H
#define STARTMENUSCENE_H

#include "cocos2d.h"
USING_NS_CC;

//when I let this row be useful and build on android ,
//static enum{	 PEACHRING=1 , PEACH, SANDIARING , SANDIA , QUITBOOMRING, QUITBOOM  }
class StartMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(StartMenu);

private:
	Point m_beginPoint;
	Point m_movePoint;


	//touch event
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	// why this function put in a tool class will cross error ????
	void removeSelf(Ref* ref);
	void creatflash();
};

#endif 
