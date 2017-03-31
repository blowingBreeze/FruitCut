#ifndef CUTCUTCUT_H
#define CUTCUTCUT_H

#include "cocos2d.h"
#include"parabola.h"
#include<vector>
USING_NS_CC;

struct st_FruitInfo
{
	String fruitImage;
	String le_cutImage;
	String rh_cutImage;
	int fruitScore;
	int Num;
};

struct st_FruitSprite
{
	Sprite* fruit;
	int fruitNum;
};

class CutCutCut : public cocos2d::Layer
{
	//friend class ParabolaAction;

public:
	CutCutCut();
	~CutCutCut();
	CREATE_FUNC(CutCutCut);
	static cocos2d::Scene* CreateScene();
	virtual bool init();

private:
	static const int TimeOfExit = 1;
	const float ThrowPointXCtl = 0.4;  //control the x range of throw point
	const float ThrowPointYCtl = 0.0; //control the y of throw point
	const float ThrowAngleCtl = 60.0; //control the angle 

	Size visibleSize;
	int m_Score;
	int m_loseCount;
	int m_TimeCount;
	Label* m_ScoreSP;
	Label*m_TimeCountSP;

	Point m_BeginPoint;
	Point m_MovePoint;
	Point m_throwPoint;
	
	std::vector<st_FruitInfo>Fruit_Contain;   //the store  of fruits'source
	std::vector<st_FruitSprite> Fruit_Manager;  // to retain the throwed fruits;
	std::vector<int>Tag_Allocator;    // to tell us which tag can be used
	
	
	void initFruitsStore();
	void throwFruit();
	virtual void update(float dt);
	void removeSelf(Ref* ref);
	void countAndClean(float dt);
	void creatflash();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	void gameOver();
	
};

#endif // !CUTCUTCUT_H