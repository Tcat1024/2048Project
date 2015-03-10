#include "GameOverLayer.h"

using namespace cocos2d;

bool GameOverLayer::init()
{
	if (!Layer::init())
		return false;
	this->setColor(Color3B(0, 0, 0));
	this->setOpacity(150);
	this->setContentSize(Director::getInstance()->getVisibleSize());
	return true;
}