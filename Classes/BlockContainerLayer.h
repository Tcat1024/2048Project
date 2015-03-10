#ifndef _BLOCKCONTAINERLAYER_H_
#define _BLOCKCONTAINERLAYER_H_

#include "cocos2d.h"
class Block;
class BlockContainerLayer :public cocos2d::Layer
{
public:
	int BlockWidth;
	int LineWidth;
	int RowCount;
	cocos2d::Color4F BackColor;	
	cocos2d::Color4F LineColor;
	static BlockContainerLayer* create(int rowcount, int blockwidth, int linewidth, cocos2d::Color4F backcolor = cocos2d::Color4F(1, 1, 0, 1))
	{
		BlockContainerLayer *pRet = new BlockContainerLayer(rowcount, blockwidth, linewidth, backcolor);
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
	virtual bool init();
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
	BlockContainerLayer(int rowcount, int blockwidth, int linewidth, cocos2d::Color4F backcolor) :RowCount(rowcount), BlockWidth(blockwidth), LineWidth(linewidth),BackColor(backcolor),Layer(){}
	cocos2d::Vec2 PointtoClient(int num);

};


#endif