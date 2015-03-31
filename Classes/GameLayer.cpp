#include "GameLayer.h"
#include "BlockContainerLayer.h"
#include "GameController.h"
#include "GameMenuLayer.h"
#include "KeyBoardListenerManager.h"

using namespace cocos2d;

bool GameScene::init()
{
	if (!Scene::init())
		return false;
	mainLayer = GameLayer::create();
	this->addChild(mainLayer);
	return true;
}
bool GameLayer::init()
{
	if (!Layer::init())
		return false;
	auto size = Director::getInstance()->getVisibleSize();
	int min = size.width>size.height ? size.height / (RowCount + ((float)RowCount + 1) / 10 + 2) : size.width / (RowCount + ((float)RowCount + 1) / 10 + 2);
	int per = min - min % 10;
	int width = (RowCount + ((float)RowCount + 1) / 10 )* per;
	auto *blockcontainer = BlockContainerLayer::create(RowCount,per,per/10);
	this->addChild(blockcontainer,0);
	blockcontainer->setContentSize(Size(width, width));
	blockcontainer->setPosition((size.width - width) / 2, (size.height - width) / 2);
	controller = new GameController(blockcontainer, RowCount);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	//KeyBoardListenerManager::Push(listener);
	return true;
}
GameLayer::~GameLayer()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	if (!controller->saved)
		controller->Save();
	if (controller != nullptr)
		delete controller;
}
void GameLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	event->stopPropagation();
	bool result = true;
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		result = this->controller->Action(ACTION_TYPE::UP);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		result = this->controller->Action(ACTION_TYPE::DOWN);
		break; 
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		result = this->controller->Action(ACTION_TYPE::LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		result = this->controller->Action(ACTION_TYPE::RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		this->addChild(GamePausedMenuLayer::create(), 10);
		break;
	default:
		break;
	}
	if (!result)
	{
		this->addChild(GameOverMenuLayer::create(), 10);
	}
}
void GameLayer::StartNewGame()
{
	this->controller->Start();
}
void GameLayer::LoadGame()
{
	this->controller->Load();
}
bool GameLayer::CheckSaveData()
{
	return GameController::CheckSave();
}
void GameLayer::SaveGame()
{
	this->controller->Save();
}