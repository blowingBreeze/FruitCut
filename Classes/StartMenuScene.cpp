#include "StartMenuScene.h"
#include "SimpleAudioEngine.h"
#include "ToolFunc.h"
#include"IDClass.h"
#include"CutCutCut.h"
#include"Audio.h"

USING_NS_CC;

Scene* StartMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//sound;
	Audio::getInstance()->playWelcome();


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//loading resource picture to cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("main.plist", "main.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fruit.plist", "fruit.png");

	//init background
	auto background = Sprite::create("background.jpg");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(background,-1);

	//add logo
	auto logo = Sprite::createWithSpriteFrameName("logo.png");
	this->addChild(logo);
	logo->setAnchorPoint(Vec2(0, 1));
	logo->setPosition(-100, 0);
	logo->runAction(EaseBounceOut::create(MoveTo::create(1, Vec2(50, visibleSize.height))));

	//add the handle board
	auto board = Sprite::createWithSpriteFrameName("home-desc.png");
	this->addChild(board);
	board->setAnchorPoint(Vec2(0, 0));
	board->setPosition(150, 280);
	board->runAction(EaseBounceOut::create(MoveTo::create(1, Vec2(150, 240))));

	//-----------------now start to add fruits and their rings------------------//

	//---------peach ring and peach-------------------//
	auto peachRing = Sprite::createWithSpriteFrameName("dojo.png");
	this->addChild(peachRing);
	peachRing->setTag(PEACHRING);
	peachRing->setPosition(150, 150);

	auto peach = Sprite::createWithSpriteFrameName("peach.png");
	peachRing->addChild(peach);
	peach->setTag(PEACH);
	peach->setPosition(peachRing->getContentSize().width / 2, peachRing->getContentSize().height / 2);
	//end peach

	//-------------add sandia and its ring-----------------------//
	auto newgameRing = Sprite::createWithSpriteFrameName("new-game.png");
	newgameRing->setTag(SANDIARING);
	this->addChild(newgameRing);
	newgameRing->setPosition(450, 230);

	auto sandia = Sprite::createWithSpriteFrameName("sandia.png");
	sandia->setTag(SANDIA);
	newgameRing->addChild(sandia);
	sandia->setPosition(newgameRing->getContentSize().width / 2, newgameRing->getContentSize().height / 2);
	//end sandia

	//--------------add boom and its ring-------------------//
	auto quitRing = Sprite::createWithSpriteFrameName("quit.png");
	quitRing->setTag(QUITBOOMRING);
	this->addChild(quitRing);
	quitRing->setPosition(650, 120);

	auto quitboom = Sprite::createWithSpriteFrameName("boom.png");
	quitboom->setTag(QUITBOOM);
	quitRing->addChild(quitboom);
	quitboom->setPosition(quitRing->getContentSize().width / 2, quitRing->getContentSize().height / 2);

	//we have to set tag for the rings because getChildByTag can only find child but not child's child;


	//------now , all the option is finished----do something more interesting--------//
	auto rotateRing = RotateBy::create(1, -90);
	auto rotateFruits = RotateBy::create(1, 180);
	peachRing->runAction(RepeatForever::create(rotateRing->clone()));
	newgameRing->runAction(RepeatForever::create(rotateRing->clone()));
	quitRing->runAction(RepeatForever::create(rotateRing->clone()));

	peach->runAction(RepeatForever::create(rotateFruits->clone()));
	sandia->runAction(RepeatForever::create(rotateFruits->clone()));
	quitboom->runAction(RepeatForever::create(rotateFruits->clone()));

	//-------------------now the rings and the fruits will rotate all the time--------------//
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(StartMenu::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(StartMenu::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(StartMenu::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

bool StartMenu::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	//when the touch begin,save the begin point;
	m_beginPoint = pTouch->getLocation();
	return true;
}

void StartMenu::onTouchMoved(Touch*pTouch, Event* pEvent)
{
	//when touch moving, save the touching point
	m_movePoint = pTouch->getLocation();

	creatflash();

	//get the boom and the ring to judge if we need to quit
	auto boomRing = static_cast<Sprite*>(this->getChildByTag(QUITBOOMRING));
	auto boom = static_cast<Sprite*>(boomRing->getChildByTag(QUITBOOM));

	auto boomRect = boom->getBoundingBox();
	auto boomRingPoint = boomRing->getPosition();
	boomRect.origin.x = boomRingPoint.x - boomRect.size.width / 2;
	boomRect.origin.y = boomRingPoint.y - boomRect.size.height / 2;

	if (boomRect.containsPoint(m_movePoint))
	{
		Audio::getInstance()->playBoom();
		auto GG = Sprite::createWithSpriteFrameName("boomlight1.png");
		auto rh_path = Sequence::create(MoveTo::create(0.1, Vec2(m_movePoint.x + 2, m_movePoint.y - 2)),
			MoveTo::create(0.1, Vec2(m_movePoint.x - 2, m_movePoint.y + 2)),
			CallFuncN::create(CC_CALLBACK_1(StartMenu::removeSelf, this)), nullptr);
		this->runAction(DelayTime::create(0.5));
		Director::getInstance()->end();
	}

	//get the ring and new game sprite;
	auto newgameRing = (Sprite*)this->getChildByTag(SANDIARING);
	auto newgame = (Sprite*)newgameRing->getChildByTag(SANDIA);

	//here we need to judge because touch event is happenning too fast ,
	//so we might handle the newgame two or more times; 
	if (newgame != nullptr)   
	{
		Audio::getInstance()->playSplatter();
		//get the boundingBox;
		auto sandiaRect = newgame->getBoundingBox();
		auto RingPoint = newgameRing->getPosition();
		sandiaRect.origin.x = RingPoint.x - sandiaRect.size.width / 2;
		sandiaRect.origin.y = RingPoint.y - sandiaRect.size.height / 2;

		//if come to the sandia than start the new game;
		if (sandiaRect.containsPoint(m_movePoint))
		{
			float rotaRing = newgameRing->getRotation();
			float rotaChil = newgame->getRotation();
			newgame->removeFromParent();  //delete itself

			//create left and right half of sandia;
			auto le_sandia = Sprite::createWithSpriteFrameName("sandia-1.png");
			auto rh_sandia = Sprite::createWithSpriteFrameName("sandia-2.png");
			le_sandia->setRotation(rotaChil + rotaRing);
			rh_sandia->setRotation(rotaChil + rotaRing);

			this->addChild(le_sandia);
			le_sandia->setPosition(RingPoint);
			this->addChild(rh_sandia);
			rh_sandia->setPosition(RingPoint);

			auto le_path = Sequence::create(MoveTo::create(1, ccp(RingPoint.x - 50.0, RingPoint.y + 50)), CallFuncN::create(CC_CALLBACK_1(StartMenu::removeSelf, this)), nullptr);
			auto rh_path = Sequence::create(MoveTo::create(1, ccp(RingPoint.x + 50.0, RingPoint.y + 50)), CallFuncN::create(CC_CALLBACK_1(StartMenu::removeSelf, this)), nullptr);

			le_sandia->runAction(le_path);
			rh_sandia->runAction(rh_path);
			this->runAction(DelayTime::create(0.5));
			//----------go to the main scene
			auto MainScene = CutCutCut::CreateScene();
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->replaceScene(MainScene);
		}
	}



	//get peach and the ring
	auto peachRing = static_cast<Sprite*>(this->getChildByTag(PEACHRING));
	auto peach = static_cast<Sprite*>(peachRing->getChildByTag(PEACH));

	auto peachRect = peach->getBoundingBox();
	auto peachRingPoint = peachRing->getPosition();
	peachRect.origin.x = peachRingPoint.x - peachRect.size.width / 2;
	peachRect.origin.y = peachRingPoint.y - peachRect.size.height / 2;

	if (peachRect.containsPoint(m_movePoint))
	{
		//just go to game end;
		//Director::getInstance()->end();
	}

	m_beginPoint = m_movePoint;
}

void StartMenu::onTouchEnded(Touch*pTouch, Event*pEvent)
{
	//when the touch end, we don't need to do anything;
	;
}

void StartMenu::removeSelf(Ref * ref)
{
	((Sprite*)ref)->removeFromParent();
}

void StartMenu::creatflash()
{
	Sprite* flash = Sprite::create("flash.png");
	float k;
	float angle;
	if (m_movePoint.x != m_beginPoint.x)
	{
		 k= (m_movePoint.y - m_beginPoint.y) / (m_movePoint.x - m_beginPoint.x);
		angle = (atan(k) / (M_PI)) * 180;
	}
	else
	{
		angle = 90;
	}
	flash->setRotation(-angle);
	this->addChild(flash);
	flash->setPosition(m_movePoint);

	auto path = Sequence::create(DelayTime::create(0.3f), CallFuncN::create(CC_CALLBACK_1(StartMenu::removeSelf, this)), nullptr);
	flash->runAction(path);
}