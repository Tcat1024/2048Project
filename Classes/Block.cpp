#include "Block.h"
using namespace cocos2d;

void Block::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	DrawPrimitives::drawSolidRect(Point(0, 0), this->getContentSize(), BackColor);
	Sprite::draw(renderer, transform, flags);
}
bool Block::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	this->setBackColor(BackColor);
	this->setAnchorPoint(Point(0, 0));
	label = Label::create(std::to_string(Index), "Arial", 48);
	this->addChild(label);
	Invincible = true;
	return true;
}
void Block::setIndex(int index)
{
	Index = index;
	if (label != nullptr)
	{
		label->setString(std::to_string(Index));
		resize();
		return;
	}
	label = Label::create(std::to_string(Index), "Arial", 48);
	this->addChild(label);
	resize();
}
void Block::onEnterTransitionDidFinish()
{
	resize();
	Sprite::onEnterTransitionDidFinish();
}
void Block::setBackColor(Color4F backcolor)
{
	this->BackColor = backcolor;
}
void Block::resize()
{
	auto size = this->getContentSize();
	if (label != nullptr)
	{
		Size temps = label->getContentSize();
		float sc = ((float)size.width / 2) / temps.height;
		label->setScale(sc);
		label->setPosition(Point(size.width/ 2,size.height/2 ));
	}
}
Block* Block::create(int index,Color4F backcolor)
{
	Block* blk = new Block(index,backcolor);
	if (blk&&blk->init())
	{
		blk->autorelease();
	}
	else
	{
		delete(blk);
		blk = nullptr;
	}
	return blk;
}

 int Block::getIndex()
{
	 return this->Index;
}