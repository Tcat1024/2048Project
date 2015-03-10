#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include "cocos2d.h"

enum ACTION_TYPE
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class BlockContainerLayer;
class Block;

class GameController
{
public:
	void Action(ACTION_TYPE type);
	void Start();
	GameController(BlockContainerLayer* checkboard, int rowcount) :checkerBoard(checkboard), rowCount(rowcount)
	{
		int maxcount = rowcount*rowcount;
		blocks = new Block*[maxcount];
		check = new bool[maxcount];
		for (int i = 0; i < maxcount; i++)
			check[i] = false;
	}
	~GameController()
	{
		Block* temp;
		for (int i = 0; i < rowCount*rowCount; i++)
		{
			if (check[i])
				delete(this->blocks[i]);
		}
		delete[] blocks;
		delete[] check;
	}
protected:
	bool newRandomBlock();
	BlockContainerLayer* checkerBoard;
	int rowCount;
	Block* newBlock(int index);
	bool fight(Block* a, Block* b);
	void addBlock(int num, Block* target);
	void moveBlock(int onum, int nnum);
	void removeBlock(int num); 
	void removeBlock(Block* target, int num);
	Block* getBlock(int num);
	int getNum(int x, int y);
	cocos2d::Color4F GameController::getColor(int index, cocos2d::Color4F color = cocos2d::Color4F(0, 0, 0, 0));
	bool* check;
	Block** blocks;
	int getFreeBlockCount();
};

#endif