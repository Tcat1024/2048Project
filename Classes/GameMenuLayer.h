#ifndef _GAMEMENULAYER_H_
#define _GAMEMENULAYER_H_

#include "cocos2d.h"
class Button
{
public:
	typedef std::function<void(void)> FUNCTYPE;
	cocos2d::Label* getLabel()
	{
		return Label;
	}
	void Clicked()
	{
		Func();
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
	Button(cocos2d::Label* const label = 0) :Label(label), Func([](){}), Enable(true)
	{
		if (Label != 0)
			Label->retain();
	}
	~Button()
	{
		if (Label != 0)
			Label->release();
	}
	bool Enable;
protected:
	cocos2d::Label* Label;
	FUNCTYPE Func;
};
class MenuLayer : public cocos2d::Layer
{
protected:
	enum
	{
		BACKGROUNDLAYER = 0,
		BUTTONLAYER = 1,
		TITLELAYER = 2
	};	
	virtual bool init()
	{
		return true;
	}
	MenuLayer() :title(0), buttons(0), backGround(0), buttoncount(0), _selectedindex(-1), originscale(1)
	{
	}
	~MenuLayer();
	int buttoncount;
	int _selectedindex;
	float originscale;
	cocos2d::Color4B origincolor;
	cocos2d::Label* title;
	Button* buttons;
	cocos2d::LayerColor* backGround;
	void Select(const int &index);
	void Access(const int &index);
	void virtual onEnterTransitionDidFinish();
	void virtual reSize();
	void virtual reSize(const cocos2d::Size& size);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
};

class GameOverMenuLayer : public MenuLayer
{
public:
	bool init();
	CREATE_FUNC(GameOverMenuLayer);

};
class GamePausedMenuLayer : public MenuLayer
{
public:
	bool init();
	CREATE_FUNC(GamePausedMenuLayer);
};

class MainMenuLayer : public MenuLayer
{
public:
	static cocos2d::Scene* createScene();
	bool init();
	CREATE_FUNC(MainMenuLayer);
};
#endif