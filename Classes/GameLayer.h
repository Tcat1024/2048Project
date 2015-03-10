#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "GameController.h"

class GameLayer:public cocos2d::Layer
{
public:
	int RowCount = 4;
	virtual bool init();
	CREATE_FUNC(GameLayer);
	GameController* controller;
	~GameLayer();
private:
	void GameLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};

#endif