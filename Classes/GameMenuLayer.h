#ifndef _GAMEMENULAYER_H_
#define _GAMEMENULAYER_H_

#include "cocos2d.h"
enum MenuMode
{
	STOP,
	OVER
};
class GameMenuLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	GameMenuLayer(MenuMode mode) :Mode(mode)
	{
	}
	~GameMenuLayer();
	static GameMenuLayer* create(MenuMode mode)
	{
		GameMenuLayer *pRet = new GameMenuLayer(mode);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
protected:
	MenuMode Mode;
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};

#endif