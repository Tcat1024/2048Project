#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"

class GameController;

class GameLayer:public cocos2d::Layer
{
public:
	int RowCount = 4;
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameLayer);
	GameController* controller;
	GameLayer() : controller(0)
	{

	}
	~GameLayer();
private:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};

#endif