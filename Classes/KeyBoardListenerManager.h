#ifndef _KEYBOARDLISTENERMANAGER_H_
#define _KEYBOARDLISTENERMANAGER_H_

#include "cocos2d.h"
#include <stack>

class KeyBoardListenerManager
{
public:
	static void Push(cocos2d::EventListenerKeyboard*);
	static void Pop();
	static void Clear();
	~KeyBoardListenerManager()
	{
		Clear();
		delete mdata;
	}
private:
	static std::stack<cocos2d::EventListenerKeyboard*>* mdata;
};
#endif