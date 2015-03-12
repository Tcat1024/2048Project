#ifndef _GAMEMENULAYER_H_
#define _GAMEMENULAYER_H_

#include "cocos2d.h"
enum MenuMode
{
	STOP,
	OVER
};
class Button
{

public:
	typedef std::function<void(void)> FUNCTYPE;
	cocos2d::Label* getLabel()
	{
		return Label;
	}
	FUNCTYPE getFunc()
	{
		return Func;
	}
	void setLabel(cocos2d::Label* const label)
	{
		if (Label != 0)
		{
			Label->release();
		}
		Label = label;
		if (Label != 0)
		{
			Label->retain();
		}
	}
	void setFunc(FUNCTYPE &func)
	{
		Func = func;
	}
	Button(cocos2d::Label* const label = 0) :Label(label)
	{
		if (Label != 0)
			Label->retain();
	}
	~Button()
	{
		if (Label != 0)
			Label->release();
	}
protected:
	cocos2d::Label* Label;
	FUNCTYPE Func;
};
class StopMenuLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(StopMenuLayer);
	~StopMenuLayer();
protected:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};
class GameOverMenuLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	GameOverMenuLayer() :title(0), buttons(0), backGround(0), buttoncount(3), _selectedindex(-1), originscale(1)
	{

	}
	CREATE_FUNC(GameOverMenuLayer);
	~GameOverMenuLayer();
protected:
	enum
	{
		BACKGROUNDLAYER = 0,
		BUTTONLAYER = 1,
		TITLELAYER = 2
	};
	int buttoncount;
	int _selectedindex;
	float originscale;
	cocos2d::Color4B origincolor;
	cocos2d::Label* title;
	Button* buttons;
	cocos2d::LayerColor* backGround;
	void Select(int index);
	void Access(int index);
	void virtual onEnterTransitionDidFinish();
	void virtual reSize();
	void virtual reSize(const cocos2d::Size& size);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};
class StartMenuLayer :public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(StartMenuLayer);
	~StartMenuLayer();
protected:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};

#endif