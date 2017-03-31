#include "CutCutCut.h"
#include"ToolFunc.h"
#include"IDClass.h"
#include"StartMenuScene.h"
#include"Audio.h"


CutCutCut::CutCutCut()
{
}


CutCutCut::~CutCutCut()
{
}

Scene * CutCutCut::CreateScene()
{
	auto scene = Scene::create();
	auto layer = CutCutCut::create();
	scene->addChild(layer);

	return scene;
}

bool CutCutCut::init()
{
	if (!Layer::init())
		return false;


	//init the data member
	srand(time(0) + 100);
	visibleSize = Director::getInstance()->getVisibleSize();

	//init background
	auto background = Sprite::create("background.jpg");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(background, -1);
	m_Score = 0;
	m_loseCount = 0;
	m_TimeCount = 60;

	//add score sprite
	m_ScoreSP = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Score:%d", m_Score)->getCString());
	m_ScoreSP->setAnchorPoint(Vec2(0, 1));
	m_ScoreSP->setPosition(Vec2(0, visibleSize.height));
	m_ScoreSP->setColor(Color3B(0, 255, 0));
	m_ScoreSP->setBMFontSize(24);
	this->addChild(m_ScoreSP);

	//add time sprite;
	m_TimeCountSP= Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Time:%d", m_TimeCount)->getCString());
	m_TimeCountSP->setAnchorPoint(Vec2(0.5, 1));
	m_TimeCountSP->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	m_TimeCountSP->setColor(Color3B(255, 0, 0));
	m_ScoreSP->setBMFontSize(24);
	this->addChild(m_TimeCountSP);

	//add the back button
	auto backbar = Sprite::create("back.png");
	addChild(backbar);
	backbar->setAnchorPoint(Vec2(1, 0));
	backbar->setPosition(visibleSize.width, 0);
	backbar->setTag(CCCUT_BACK);

	//init the grey cross at the top right corner
	auto x = Sprite::createWithSpriteFrameName("x.png");
	x->setAnchorPoint(Vec2(1, 1));
	this->addChild(x);
	x->setPosition(720, visibleSize.height);

	auto xx = Sprite::createWithSpriteFrameName("xx.png");
	xx->setAnchorPoint(Vec2(1, 1));
	this->addChild(xx);
	xx->setPosition(745, visibleSize.height);

	auto xxx = Sprite::createWithSpriteFrameName("xxx.png");
	xxx->setAnchorPoint(Vec2(1, 1));
	this->addChild(xxx);
	xxx->setPosition(775, visibleSize.height);
	//the cross init finished

	//init fruits store
	initFruitsStore();

	//listen the touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(CutCutCut::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(CutCutCut::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(CutCutCut::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//game over listener
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto countListener = EventListenerCustom::create(GAMEOVER, [=](EventCustom* eventCustom) { gameOver(); });
	dispatcher->addEventListenerWithSceneGraphPriority(countListener, this);

	schedule(schedule_selector(CutCutCut::update), 0.5);

	return true;
}

//add the fruits to the container
void CutCutCut::initFruitsStore()
{
	st_FruitInfo tempFruit;
	tempFruit.fruitImage = "peach.png";
	tempFruit.le_cutImage = "peach-1.png";
	tempFruit.rh_cutImage = "peach-2.png";
	tempFruit.fruitScore = 1;
	tempFruit.Num = 0;
	Fruit_Contain.push_back(tempFruit);

	tempFruit.fruitImage = "apple.png";
	tempFruit.le_cutImage = "apple-1.png";
	tempFruit.rh_cutImage = "apple-2.png";
	tempFruit.fruitScore = 2;
	tempFruit.Num = 1;
	Fruit_Contain.push_back(tempFruit);

	tempFruit.fruitImage = "banana.png";
	tempFruit.le_cutImage = "banana-1.png";
	tempFruit.rh_cutImage = "banana-2.png";
	tempFruit.fruitScore = 3;
	tempFruit.Num = 2;
	Fruit_Contain.push_back(tempFruit);

	tempFruit.fruitImage = "basaha.png";
	tempFruit.le_cutImage = "basaha-1.png";
	tempFruit.rh_cutImage = "basaha-2.png";
	tempFruit.fruitScore = 3;
	tempFruit.Num = 3;
	Fruit_Contain.push_back(tempFruit);

	tempFruit.fruitImage = "sandia.png";
	tempFruit.le_cutImage = "sandia-1.png";
	tempFruit.rh_cutImage = "sandia-2.png";
	tempFruit.fruitScore = 1;
	tempFruit.Num = 4;
	Fruit_Contain.push_back(tempFruit);

	tempFruit.fruitImage = "boom.png";
	tempFruit.le_cutImage = "boomlight1.png";
	tempFruit.rh_cutImage = "boomlight1.png";
	tempFruit.fruitScore = 0;
	tempFruit.Num = 5;
	Fruit_Contain.push_back(tempFruit);
}


void CutCutCut::throwFruit()
{
	int subOfThrow = rand() % Fruit_Contain.size();
	st_FruitInfo tempFruitInfo = Fruit_Contain[subOfThrow];

	auto tempFruitSP = Sprite::createWithSpriteFrameName(tempFruitInfo.fruitImage.getCString());
	st_FruitSprite tempStFruitSprite;
	tempStFruitSprite.fruit = tempFruitSP;
	tempStFruitSprite.fruitNum = tempFruitInfo.Num;
	Fruit_Manager.push_back(tempStFruitSprite);
	this->addChild(tempFruitSP);

	//where the fruits come out
	m_throwPoint.x = visibleSize.width*ThrowPointXCtl + rand() % (int(visibleSize.width*ThrowPointXCtl));
	tempFruitSP->setPosition(Vec2(m_throwPoint.x, ThrowPointYCtl));

	//rotate director
	int counterOrNot = 0;
	(m_throwPoint.x > visibleSize.width / 2) ? counterOrNot = ANTICLOCKWISE : counterOrNot = CLOCKWISE;

	//caculate for the parabola
	auto angle = ThrowAngleCtl + rand() % (int(90 - ThrowAngleCtl));    //throw angle
	auto throwHeight = visibleSize.height / 2 + rand() % (int(visibleSize.height / 2));
	auto horizenV = -counterOrNot*visibleSize.width *(1 - 2 * ThrowPointXCtl) / TimeOfExit;            //horizen speed;
	auto verticalV = throwHeight * 2 / TimeOfExit;    //vertical speed;
	float gravity = 2 * throwHeight / (TimeOfExit*TimeOfExit);

	//some action
	auto rota = RotateBy::create(3, 360 * counterOrNot);   //rotate action
	auto Parabola = ParabolaAction::create(m_throwPoint, Vec2(horizenV, verticalV), gravity, TimeOfExit*2.5);
	auto spawn = Spawn::create(rota, Parabola, nullptr);

	//run these action at the same time
	tempFruitSP->runAction(spawn);
	Audio::getInstance()->playThrow();
}

bool CutCutCut::onTouchBegan(Touch * pTouch, Event * pEvent)
{
	m_BeginPoint = pTouch->getLocation();
	//judge for the back button
	auto back = this->getChildByTag(CCCUT_BACK);
	auto rectBack = back->getBoundingBox();

	if (rectBack.containsPoint(m_BeginPoint))
	{
		auto startMenuScene = StartMenu::createScene();
		Director::getInstance()->replaceScene(startMenuScene);
	}
	return true;
}

void CutCutCut::onTouchMoved(Touch * pTouch, Event * pEvent)
{
	m_MovePoint = pTouch->getLocation();
	creatflash();
#pragma region here to judge whether we cut the fruit
	for (auto it = Fruit_Manager.begin();it!=Fruit_Manager.end();)
	{
		auto obj = *it;
		auto tempFruitSprite = obj.fruit;
		if (tempFruitSprite->getBoundingBox().containsPoint(m_MovePoint))
		{
			if (5 == obj.fruitNum)
			{
				Audio::getInstance()->playBoom();
				//here , GameOVER
				auto angle = ToolFunc::getRotaAngle(m_BeginPoint, m_MovePoint);
				auto rh_fruit = Sprite::createWithSpriteFrameName(Fruit_Contain[obj.fruitNum].rh_cutImage.getCString());
				rh_fruit->setRotation(angle + tempFruitSprite->getRotation());
				rh_fruit->setPosition(tempFruitSprite->getPosition());
				auto rh_path = Sequence::create(MoveTo::create(0.1, Vec2(m_MovePoint.x + 2, m_MovePoint.y -2)),
					MoveTo::create(0.1, Vec2(m_MovePoint.x - 2, m_MovePoint.y + 2)),
					CallFuncN::create(CC_CALLBACK_1(CutCutCut::removeSelf, this)), nullptr);

				this->addChild(rh_fruit);
				rh_fruit->runAction(rh_path);

				EventCustom event(GAMEOVER);
				auto dispatcher = Director::getInstance()->getEventDispatcher();
				dispatcher->dispatchEvent(&event);
				break;

				//-----------------------------------------------
			}

			Audio::getInstance()->playSplatter();
			// it was cut ,sou we enhance the score;
			m_Score += Fruit_Contain[obj.fruitNum].fruitScore;

			//the fruits were cut, so we remove them ,and we should not delete it twice
			//tempFruitSprite->removeFromParent();

			auto angle = ToolFunc::getRotaAngle(m_BeginPoint, m_MovePoint);
			auto le_fruit = Sprite::createWithSpriteFrameName(Fruit_Contain[obj.fruitNum].le_cutImage.getCString());
			auto rh_fruit = Sprite::createWithSpriteFrameName(Fruit_Contain[obj.fruitNum].rh_cutImage.getCString());
			le_fruit->setRotation(angle + tempFruitSprite->getRotation());
			rh_fruit->setRotation(angle + tempFruitSprite->getRotation());
			le_fruit->setAnchorPoint(Vec2(1, 0.5));
			rh_fruit->setAnchorPoint(Vec2(0, 0.5));
			le_fruit->setPosition(tempFruitSprite->getPosition());
			rh_fruit->setPosition(tempFruitSprite->getPosition());

			this->addChild(le_fruit);
			this->addChild(rh_fruit);

			auto le_path = Sequence::create(MoveTo::create(1.0, Vec2(m_MovePoint.x - 50, m_throwPoint.y)), CallFuncN::create(CC_CALLBACK_1(CutCutCut::removeSelf, this)), nullptr);
			auto rh_path = Sequence::create(MoveTo::create(1.0, Vec2(m_MovePoint.x + 50, m_throwPoint.y)), CallFuncN::create(CC_CALLBACK_1(CutCutCut::removeSelf, this)), nullptr);
			le_fruit->runAction(le_path);
			rh_fruit->runAction(rh_path);

			tempFruitSprite->removeFromParent();
			it = Fruit_Manager.erase(it);
			continue;
		}
		++it;
	}
#pragma endregion

#pragma region here we judge whether we touch the back button
	auto back = this->getChildByTag(CCCUT_BACK);
	auto rectBack = back->getBoundingBox();

	if (rectBack.containsPoint(m_MovePoint))
	{
		auto startMenuScene = StartMenu::createScene();
		Director::getInstance()->replaceScene(startMenuScene);
	}

#pragma endregion

	m_BeginPoint = m_MovePoint;
}

void CutCutCut::onTouchEnded(Touch * pTouch, Event * pEvent)
{
	;
}



void CutCutCut::update(float dt)
{
	throwFruit();
	countAndClean(dt);
	m_TimeCount -= dt;
	m_ScoreSP->setString(String::createWithFormat("Score:%d", m_Score)->getCString());
	m_TimeCountSP->setString(String::createWithFormat("Time:%d", m_TimeCount)->getCString());

	if (m_TimeCount <= 0)
	{
		EventCustom event(GAMEOVER);
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->dispatchEvent(&event);
	}

}

void CutCutCut::removeSelf(Ref * ref)
{
	((Sprite*)ref)->removeFromParent();
}


void CutCutCut::countAndClean(float dt)
{
	if (Fruit_Manager.size() <= 0) return;
	for (auto it= Fruit_Manager.begin();it!=Fruit_Manager.end();++it)
	{
		auto obj = *it;
		auto tempFruitSprite = obj.fruit;
		if(tempFruitSprite->getPosition().y<0)
		{
			if(5!=obj.fruitNum)
				++m_loseCount;
			it=Fruit_Manager.erase(it);
		}
	}
	//every frame we check if gameOver;
	if (1 == m_loseCount)
	{
		auto xf = Sprite::createWithSpriteFrameName("xf.png");
		xf->setPosition(720, visibleSize.height);
		xf->setAnchorPoint(Vec2(1, 1));
		this->addChild(xf);
	}
	if (2 == m_loseCount)
	{
		auto xxf = Sprite::createWithSpriteFrameName("xxf.png");
		xxf->setPosition(745, visibleSize.height);
		xxf->setAnchorPoint(Vec2(1, 1));
		this->addChild(xxf);
	}
	if (3 == m_loseCount)
	{
		auto xxxf = Sprite::createWithSpriteFrameName("xxxf.png");
		xxxf->setPosition(775, visibleSize.height);
		xxxf->setAnchorPoint(Vec2(1, 1));
		this->addChild(xxxf);

		EventCustom event(GAMEOVER);
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->dispatchEvent(&event);
	}

}


void CutCutCut::gameOver()
{
	unschedule(schedule_selector(CutCutCut::update));
	auto gg = Sprite::createWithSpriteFrameName("game-over.png");
	this->addChild(gg, 10);
	gg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	auto comeOut = Sequence::create(TintBy::create(2, 180, 180, 180), nullptr);
	gg->runAction(comeOut); 
}

void CutCutCut::creatflash()
{
	Sprite* flash = Sprite::create("flash.png");
	float k;
	float angle;
	if (m_MovePoint.x != m_BeginPoint.x)
	{
		k = (m_MovePoint.y - m_BeginPoint.y) / (m_MovePoint.x - m_BeginPoint.x);
		angle = (atan(k) / (M_PI)) * 180;
	}
	else
	{
		angle = 90;
	}
	flash->setRotation(-angle);
	this->addChild(flash);
	flash->setPosition(m_MovePoint);

	auto path = Sequence::create(DelayTime::create(0.3f), CallFuncN::create(CC_CALLBACK_1(CutCutCut::removeSelf, this)), nullptr);
	flash->runAction(path);
}