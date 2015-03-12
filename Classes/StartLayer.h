#ifndef _STARTLAYER_H_
#define _STARTLAYER_H_
#include "cocos2d.h"

class StartLayer : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	~StartLayer()
	{
	}
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(StartLayer);
};
#endif // !_STARTLAYER_
