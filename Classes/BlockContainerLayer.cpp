#include "BlockContainerLayer.h"
#include "Block.h"
using namespace cocos2d;

void BlockContainerLayer::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	auto size = this->getContentSize();
	float width = size.height;
	DrawPrimitives::drawSolidRect(Point(0, 0), size, BackColor);
	for (int i = 0; i <RowCount+1; i++)
	{
		
		DrawPrimitives::drawSolidRect(Point(i*(LineWidth + BlockWidth), 0), Point(i*(LineWidth + BlockWidth) + LineWidth, width), LineColor);
		DrawPrimitives::drawSolidRect(Point(0, i*(LineWidth + BlockWidth)), Point(width, i*(LineWidth + BlockWidth) + LineWidth), LineColor);
	}
	Layer::draw(renderer, transform, flags);
}
bool BlockContainerLayer::init()
{
	if (!Layer::init())
		return false;
	LineColor = Color4F(BackColor.r / 2, BackColor.g / 2, BackColor.b / 2, 1);	
	return true;

}
cocos2d::Vec2 BlockContainerLayer::PointtoClient(int num)
{
	int col = num%RowCount;
	int row = num / RowCount;
	return cocos2d::Vec2((col+1)*LineWidth+col*BlockWidth, (row+1)*LineWidth+row*BlockWidth);
}