#include "GameLayer.h"
#include "BlockContainerLayer.h"

using namespace cocos2d;


bool GameLayer::init()
{
	if (!Layer::init())
		return false;
	auto size = Director::getInstance()->getVisibleSize();
	int min = size.width>size.height ? size.height / (RowCount + ((float)RowCount + 1) / 10 + 2) : size.width / (RowCount + ((float)RowCount + 1) / 10 + 2);
	int per = min - min % 10;
	int width = (RowCount + ((float)RowCount + 1) / 10 )* per;
	auto *blockcontainer = BlockContainerLayer::create(RowCount,per,per/10);
	this->addChild(blockcontainer);
	blockcontainer->setContentSize(Size(width, width));
	blockcontainer->setPosition((size.width - width) / 2, (size.height - width) / 2);
	controller = new GameController(blockcontainer, RowCount);
	controller->Start();
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}
GameLayer::~GameLayer()
{
	if (controller != 0)
		delete controller;
	Layer::~Layer();
}
void GameLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		((GameLayer*)event->getCurrentTarget())->controller->Action(ACTION_TYPE::UP);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		((GameLayer*)event->getCurrentTarget())->controller->Action(ACTION_TYPE::DOWN);
		break; 
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		((GameLayer*)event->getCurrentTarget())->controller->Action(ACTION_TYPE::LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		((GameLayer*)event->getCurrentTarget())->controller->Action(ACTION_TYPE::RIGHT);
		break;
	default:
		break;
	}
}