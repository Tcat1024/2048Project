#include "GameController.h"
#include "Block.h"
#include "BlockContainerLayer.h"
#include "GameOverLayer.h"
#include "sqlite3.h"
using namespace cocos2d;

void GameController::Start()
{
	this->checkerBoard->removeAllChildrenWithCleanup(true);
	Reset();
	newRandomBlock();
	newRandomBlock();
}
void GameController::Reset()
{
	for (int i = 0; i < rowCount*rowCount; i++)
		check[i] = false;
}
Block* GameController::newBlock(int index,Color4F color)
{
	return Block::create(index, color);
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
	target->setContentSize(Size(checkerBoard->BlockWidth, checkerBoard->BlockWidth));
	this->checkerBoard->addChild(target);
	target->setPosition(checkerBoard->PointtoClient(num));
	this->check[num] = true;
	this->blocks[num] = target;
}
void GameController::moveBlock(int onum, int nnum)
{
	Block* temp = this->blocks[nnum];
	this->blocks[nnum] = this->blocks[onum];
	auto moveto = MoveTo::create(0.05, checkerBoard->PointtoClient(nnum));
	this->lock++;
	if (this->check[nnum])
	{
		auto actionMoveDone = CallFunc::create([this, temp](){ this->checkerBoard->removeChild(temp); this->lock--; });
		this->blocks[nnum]->runAction(Sequence::create(moveto, actionMoveDone,NULL));
		//checkerBoard->removeChild(temp);
	}
	else
	{
		auto actionMoveDone = CallFunc::create([this](){ this->lock--; });
		this->blocks[nnum]->runAction(Sequence::create(moveto, actionMoveDone,NULL));
	}
	//this->blocks[nnum]->setPosition(checkerBoard->PointtoClient(nnum));
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
	int num;
	count = 1;
	for (int i = 0; i < rowCount*rowCount; i++)
	{
		if (!check[i] && count++ == index)
		{
			num = random(1, 2) * 2;
			addBlock(i, newBlock(num,getColor(num)));
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
bool GameController::Action(ACTION_TYPE type)
{
	if (lock != 0)
		return true;
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
								//removeBlock(tnum);
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
		saved = false;
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
								//removeBlock(tnum);
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
		saved = false;
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
								//removeBlock(tnum);
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
		saved = false;
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
								//removeBlock(tnum);
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
		saved = false;
		break;
	default:
		break;
	}
	if (has)
	{
		if (!newRandomBlock())
			return false;
	}
	else if (getFreeBlockCount() == 0&&checkGameOver())
	{
		return false;
	}
	return true;
}
bool GameController::checkGameOver()
{
	int i, j;
	for (i = 0; i < rowCount-1; i++)
	{
		for (j = 0; j < rowCount-1; j++)
		{
			if (this->blocks[i*rowCount + j]->getIndex() == this->blocks[i*rowCount + j + 1]->getIndex() || this->blocks[i*rowCount + j]->getIndex() == this->blocks[i*rowCount + j + rowCount]->getIndex())
			{
				return false;
			}
		}
		if (this->blocks[i*rowCount + rowCount - 1]->getIndex() == this->blocks[i*rowCount + rowCount - 1 + rowCount]->getIndex())
			return false;
	}
	for (j = 0; j < rowCount - 1; j++)
	{
		if (this->blocks[i*rowCount + j]->getIndex() == this->blocks[i*rowCount + j + 1]->getIndex())
		{
			return false;
		}
	}
	return true;
}
void GameController::Save()
{
	sqlite3 *db;
	int result;
	String path = CCFileUtils::getInstance()->getWritablePath() + "Data";
	char** table = NULL;
	char * errormessage = NULL;
	int r, c;
	if ((result = sqlite3_open(path.getCString(), &db)) != SQLITE_OK)
	{
		log("Open database failed, error:%d", result);
		return;
	}
	do
	{
		if ((result = sqlite3_get_table(db, "SELECT * FROM sqlite_master where type='table' and name='savedata'", &table, &r, &c, &errormessage)) != SQLITE_OK)
		{
			log("Quary table failed, error:%s", errormessage);
			break;
		}
		if (r == 0 && (result = sqlite3_exec(db, "create table savedata(num integer,ind integer,R real,G real,B real)", NULL, NULL, &errormessage)) != SQLITE_OK)
		{
			log("Create table failed, error:%s", errormessage);
			break;
		}
		if (r != 0 && (result = sqlite3_exec(db, "delete from saveData", NULL, NULL, &errormessage)) != SQLITE_OK)
		{
			log("Delete table failed, error:%s", errormessage);
			break;
		}
		int maxcount = rowCount*rowCount;
		Block *temp;
		String insertstring;
		for (int i = 0; i < maxcount; i++)
		{
			if (check[i])
			{
				temp = blocks[i];
				insertstring.initWithFormat("insert into savedata values (%d,%d,%.3f,%.3f,%.3f)", i, temp->getIndex(), temp->BackColor.r, temp->BackColor.g, temp->BackColor.b);
				if ((result = sqlite3_exec(db, insertstring.getCString(), NULL, NULL, &errormessage)) != SQLITE_OK)
				{
					log("Insert table failed, error:%s", errormessage);
					delete errormessage;
					errormessage = NULL;
					if ((result = sqlite3_exec(db, "drop savedata", NULL, NULL, &errormessage)) != SQLITE_OK)
					{
						log("Drop table failed, error:%s", errormessage);
					}
					break;
				}
			}
		}
	} while (0);
	if (table != NULL)
		sqlite3_free_table(table);
	if (errormessage != NULL)
		delete errormessage;
	if ((result = sqlite3_close(db)) != SQLITE_OK)
	{
		log("Close database failed, error:%s", errormessage);
		delete errormessage;
		return;
	}
	saved = true;
}
void GameController::Load()
{
	sqlite3 *db;
	int result;
	String path = CCFileUtils::getInstance()->getWritablePath() + "Data";
	char** table = NULL;
	char * errormessage = NULL;
	int r, c;
	if ((result = sqlite3_open(path.getCString(), &db)) != SQLITE_OK)
	{
		log("Open database failed, error:%d", result);
		return;
	}
	do
	{
		if ((result = sqlite3_get_table(db, "SELECT * FROM sqlite_master where type='table' and name='savedata'", &table, &r, &c, &errormessage)) != SQLITE_OK)
		{
			log("Quary table failed, error:%s", errormessage);
			break;
		}
		if (r == 0)
		{
			Start();
			break;
		}
		sqlite3_free_table(table);
		if ((result = sqlite3_get_table(db, "SELECT * FROM savedata", &table, &r, &c, &errormessage)) != SQLITE_OK)
		{
			log("Quary table failed, error:%s", errormessage);
			break;
		}
		this->checkerBoard->removeAllChildrenWithCleanup(true);
		Reset();
		for (int i = c; i < (r+1)*c; i+=c)
		{
			this->addBlock(atoi(table[i]), this->newBlock(atoi(table[i+1]), Color4F(atof(table[i+2]), atof(table[i+3]), atof(table[i+4]), 1)));
		}
	} while (0);
	if (table != NULL)
		sqlite3_free_table(table);
	if (errormessage != NULL)
		delete errormessage;
	if ((result = sqlite3_close(db)) != SQLITE_OK)
	{
		log("Close database failed, error:%s", errormessage);
		delete errormessage;
		return;
	}
}
bool GameController::CheckSave()
{
	sqlite3 *db;
	int result;
	bool re = false;
	String path = CCFileUtils::getInstance()->getWritablePath() + "Data";
	char** table = NULL;
	char * errormessage = NULL;
	int r, c;
	if ((result = sqlite3_open(path.getCString(), &db)) != SQLITE_OK)
	{
		log("Open database failed, error:%d", result);
		return false;
	}
	do
	{
		if ((result = sqlite3_get_table(db, "SELECT * FROM sqlite_master where type='table' and name='savedata'", &table, &r, &c, &errormessage)) != SQLITE_OK)
		{
			log("Quary table failed, error:%s", errormessage);
			break;
		}
		if (r != 0)
			re = true;
	} while (0);
	if (table != NULL)
		sqlite3_free_table(table);
	if (errormessage != NULL)
		delete errormessage;
	if ((result = sqlite3_close(db)) != SQLITE_OK)
	{
		log("Close database failed, error:%s", errormessage);
		delete errormessage;
	}
	return re;
}