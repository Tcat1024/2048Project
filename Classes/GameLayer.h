#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"

class GameController;
class GameScene;

class GameLayer:public cocos2d::Layer
{
public:
	int RowCount = 4;
	virtual bool init();
	CREATE_FUNC(GameLayer);
	GameController* controller;
	GameLayer() : controller(0)
	{

	}
	void StartNewGame();
	void LoadGame();
	void SaveGame();
	static bool CheckSaveData();
	~GameLayer();
private:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};
class GameScene :public cocos2d::Scene
{
public:
	CREATE_FUNC(GameScene);
	virtual bool init();
	GameLayer *mainLayer;
};
#endif