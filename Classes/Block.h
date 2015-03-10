#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "cocos2d.h"

class Block : public cocos2d::Sprite
{
public:
	cocos2d::Color4F BackColor;
	virtual bool init();
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
	Block(int index, cocos2d::Color4F backcolor) :Index(index), BackColor(backcolor)
	{
	};
	virtual void setIndex(int index);
	virtual int getIndex();
	virtual void setBackColor(cocos2d::Color4F backcolor);
	static Block* create(int index, cocos2d::Color4F backcolor);
	void resize();
	bool Invincible;
protected:
	cocos2d::Label* label = nullptr;
	int Index;
	int Width;
};
#endif