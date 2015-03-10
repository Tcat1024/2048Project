#include "StartLayer.h"
#include "GameLayer.h"

using namespace cocos2d;

cocos2d::Scene* StartLayer::createScene()
{
	auto *scene = Scene::create();
	auto *layer = StartLayer::create();
	scene->addChild(layer);
	return scene;
}
bool StartLayer::init()
{
	this->addChild(GameLayer::create());
	return true;
}
void StartLayer::menuCloseCallback(cocos2d::Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}