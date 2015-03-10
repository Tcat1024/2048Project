#include "GameMenuLayer.h"
#include "KeyBoardListenerManager.h"

using namespace cocos2d;

bool GameMenuLayer::init()
{
	if (!CCLayer::init())
		return false;
	LayerColor* layer = LayerColor::create();
	layer->setColor(Color3B(0, 0, 0));
	layer->setOpacity(150);
	layer->setContentSize(Director::getInstance()->getVisibleSize());
	this->addChild(layer);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameMenuLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 30);
	KeyBoardListenerManager::Push(listener);
	switch (this->Mode)
	{
	case MenuMode::OVER:
		break;
	case MenuMode::STOP:
		break;
	default:
		break;
	}
	return true;
}
void GameMenuLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	bool result = true;
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;
	default:
		break;
	}
	if (!result)
	{
		this->addChild(GameMenuLayer::create(MenuMode::OVER), 10);
	}
}
GameMenuLayer::~GameMenuLayer()
{
	KeyBoardListenerManager::Pop();
}