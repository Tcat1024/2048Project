#include "GameController.h"
#include "Block.h"
#include "BlockContainerLayer.h"
#include "HelloWorldScene.h"
using namespace cocos2d;

void GameController::Start()
{
	newRandomBlock();
	newRandomBlock();
}
Block* GameController::newBlock(int index)
{
	return Block::create(index, getColor(index));
}
Color4F GameController::getColor(int index,Color4F color)
{
	if (index == 2)
		return Color4F(0.8, 0.8, 0.8, 1);
	if (index == 4)
		return Color4F(1, 0.8, 0.8, 1);
	if (index == 8)
		return Color4F(1, 0.6, 0.6, 1);
	if (index == 16)
		return Color4F(1, 0.4, 0.4, 1);
	float cc[] = { color.r, color.g, color.b };
	int max = 0, maxcount = 0, min = 0,mincount=0;
	int i, j;
	for (i = 0; i < 3; i++)
	{
		if (std::round(cc[i]*10) == 10)
		{
			max = i;
			maxcount++;
		}
		else if (std::round(cc[i] * 10) == 4)
		{
			min = i;
			mincount++;
		}
	}
	if (maxcount == 2)
	{
		cc[(min + 1) % 3] -=0.2;
	}
	else if (mincount==2)
	{
		cc[(max + 1) % 3] += 0.2;
	}
	else if (min == (max+1)%3)
	{
		cc[(min + 1) % 3] -= 0.2;
	}
	else
	{
		cc[(max + 1) % 3] += 0.2;
	}
	return Color4F(cc[0], cc[1], cc[2], 1);
}
void GameController::addBlock(int num, Block* target)
{
	this->checkerBoard->addChild(target);
	target->setContentSize(Size(checkerBoard->BlockWidth, checkerBoard->BlockWidth));
	target->setPosition(checkerBoard->PointtoClient(num));
	target->resize();
	this->check[num] = true;
	this->blocks[num] = target;
}
void GameController::moveBlock(int onum, int nnum)
{
	Block* temp = this->blocks[onum];
	this->blocks[nnum] = temp;
	temp->setPosition(checkerBoard->PointtoClient(nnum));
	this->check[onum] = false;
	this->check[nnum] = true;
}
void GameController::removeBlock(int num)
{
	Block* target = this->blocks[num];
	checkerBoard->removeChild(target);
	//delete(target);
	this->check[num] = false;
}
void GameController::removeBlock(Block* target,int num)
{
	checkerBoard->removeChild(target);
	delete(target);
	this->check[num] = false;
}
Block* GameController::getBlock(int num)
{
	return this->blocks[num];
}
int GameController::getNum(int x, int y)
{
	return x*rowCount + y;
}
bool GameController::fight(Block* a, Block* b)
{
	if (b->Invincible || a->getIndex() != b->getIndex())
	{
		return false;
	}
	return true;
}
bool GameController::newRandomBlock()
{
	int count = getFreeBlockCount();
	if (count == 0)
		return false;
	int index = random(1, count);
	count = 1;
	for (int i = 0; i < rowCount*rowCount; i++)
	{
		if (!check[i] && count++ == index)
		{
			addBlock(i, newBlock(random(1,2)*2));
			return true;
		}
	}
	return false;
}
int GameController::getFreeBlockCount()
{
	int count = 0;
	for (int i = 0; i < rowCount*rowCount; i++)
	{
		if (!check[i])
			count++;
	}
	return count;
}
void GameController::Action(ACTION_TYPE type)
{
	int maxcount = rowCount*rowCount, i = 0,j=0,col=0,row=0,bnum,tnum,index;
	bool has = false;
	switch (type)
	{
	case ACTION_TYPE::DOWN:
		for (col = 0; col < rowCount; col++)
		{
			bnum = col;
			if (check[bnum])
			{
				this->blocks[bnum]->Invincible = false;
			}
			for (row = 1; row < rowCount; row++)
			{
				bnum = row*rowCount + col;
				if (check[bnum])
				{
					this->blocks[bnum]->Invincible = false;
					for (j = row - 1; j >= 0; j--)
					{
						tnum = j*rowCount + col;
						if (check[tnum])
						{
							if (fight(this->blocks[bnum], this->blocks[tnum]))
							{
								removeBlock(tnum);
								moveBlock(bnum, tnum);
								index = this->blocks[tnum]->getIndex() * 2;
								this->blocks[tnum]->setIndex(index);
								this->blocks[tnum]->setBackColor(getColor(index, this->blocks[tnum]->BackColor));
								this->blocks[tnum]->Invincible = true;
								has = true;
							}
							else if (j != row - 1)
							{
								moveBlock(bnum, tnum + rowCount);
								has = true;
							}
							break;
						}
					}
					if (j == -1&&row!=0)
					{
						moveBlock(bnum, col);
						has = true;
					}
				}
			}	
		}
		break;
	case ACTION_TYPE::UP:
		for (col = 0; col < rowCount; col++)
		{
			bnum = (rowCount-1)*rowCount + col;
			if (check[bnum])
			{
				this->blocks[bnum]->Invincible = false;
			}
			for (row = rowCount-2; row >=0; row--)
			{
				bnum = row*rowCount + col;
				if (check[bnum])
				{
					this->blocks[bnum]->Invincible = false;
					for (j = row + 1; j <= rowCount-1; j++)
					{
						tnum = j*rowCount + col;
						if (check[tnum])
						{
							if (fight(this->blocks[bnum], this->blocks[tnum]))
							{
								removeBlock(tnum);
								moveBlock(bnum, tnum);
								index = this->blocks[tnum]->getIndex() * 2;
								this->blocks[tnum]->setIndex(index);
								this->blocks[tnum]->setBackColor(getColor(index, this->blocks[tnum]->BackColor));
								this->blocks[tnum]->Invincible = true;
								has = true;
							}
							else if (j != row + 1)
							{
								moveBlock(bnum, tnum - rowCount);
								has = true;
							}
							break;
						}
					}
					if (j == rowCount)
					{
						moveBlock(bnum, col + rowCount*(rowCount - 1));
						has = true;
					}
				}
			}
		}
		break;
	case ACTION_TYPE::LEFT:
		for (row = 0; row < rowCount; row++)
		{
			bnum = row*rowCount;
			if (check[bnum])
			{
				this->blocks[bnum]->Invincible = false;
			}
			for (col = 1; col < rowCount; col++)
			{
				bnum = row*rowCount + col;
				if (check[bnum])
				{
					this->blocks[bnum]->Invincible = false;
					for (j = col - 1; j >= 0; j--)
					{
						tnum = row*rowCount + j;
						if (check[tnum])
						{
							if (fight(this->blocks[bnum], this->blocks[tnum]))
							{
								removeBlock(tnum);
								moveBlock(bnum, tnum);
								index = this->blocks[tnum]->getIndex() * 2;
								this->blocks[tnum]->setIndex(index);
								this->blocks[tnum]->setBackColor(getColor(index, this->blocks[tnum]->BackColor));
								this->blocks[tnum]->Invincible = true;
								has = true;
							}
							else if (j != col - 1)
							{
								moveBlock(bnum, tnum + 1);
								has = true;
							}
							break;
						}
					}
					if (j == -1)
					{
						moveBlock(bnum, row*rowCount);
						has = true;
					}
				}
			}
		}
		break;
	case ACTION_TYPE::RIGHT:
		for (row = 0; row < rowCount; row++)
		{
			bnum = row*rowCount + rowCount-1;
			if (check[bnum])
			{
				this->blocks[bnum]->Invincible = false;
			}
			for (col = rowCount - 2; col >= 0; col--)
			{
				bnum = row*rowCount + col;
				if (check[bnum])
				{
					this->blocks[bnum]->Invincible = false;
					for (j = col + 1; j <= rowCount - 1; j++)
					{
						tnum = row*rowCount + j;
						if (check[tnum])
						{
							if (fight(this->blocks[bnum], this->blocks[tnum]))
							{
								removeBlock(tnum);
								moveBlock(bnum, tnum);
								index = this->blocks[tnum]->getIndex() * 2;
								this->blocks[tnum]->setIndex(index);
								this->blocks[tnum]->setBackColor(getColor(index, this->blocks[tnum]->BackColor));
								this->blocks[tnum]->Invincible = true;
								has = true;
							}
							else if (j != col + 1)
							{
								moveBlock(bnum, tnum - 1);
								has = true;
							}
							break;
						}
					}
					if (j == rowCount)
					{
						moveBlock(bnum, row*rowCount + rowCount - 1);
						has = true;
					}
				}
			}
		}
		break;
	default:
		break;
	}
	if (has)
	{
		if (!newRandomBlock())
			Director::getInstance()->replaceScene(HelloWorld::createScene());
	}
	else if (getFreeBlockCount() == 0)
	{
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	}
}