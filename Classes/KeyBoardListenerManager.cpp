#include "KeyBoardListenerManager.h"

using namespace cocos2d;

std::stack<cocos2d::EventListenerKeyboard*>* KeyBoardListenerManager::mdata = new std::stack<cocos2d::EventListenerKeyboard*>();
void KeyBoardListenerManager::Push(EventListenerKeyboard* target)
{
	if (KeyBoardListenerManager::mdata->size() != 0)
	{
		auto top = KeyBoardListenerManager::mdata->top();
		if (top != nullptr)
			top->setEnabled(false);
	}
	KeyBoardListenerManager::mdata->push(target);
	target->retain();
}
void KeyBoardListenerManager::Pop()
{
	if (KeyBoardListenerManager::mdata->size() != 0)
	{
		auto top = KeyBoardListenerManager::mdata->top();
		KeyBoardListenerManager::mdata->pop();
		top->release();
		if (KeyBoardListenerManager::mdata->size() != 0)
		{
			KeyBoardListenerManager::mdata->top()->setEnabled(true);
		}
	}
}
void KeyBoardListenerManager::Clear()
{
	while (KeyBoardListenerManager::mdata->size() != 0)
	{
		KeyBoardListenerManager::mdata->top()->release();
		KeyBoardListenerManager::mdata->pop();
	}
}