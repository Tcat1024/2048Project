#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"

class GameController;
class GameMenuLayer;

class GameLayer:public cocos2d::Layer
{
public:
	int RowCount = 4;
	virtual bool init();
	CREATE_FUNC(GameLayer);
	GameController* controller;
	GameMenuLayer* gameMenuLayer;
	GameLayer() : gameMenuLayer(0), controller(0)
	{

	}
	~GameLayer();
private:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};

#endif