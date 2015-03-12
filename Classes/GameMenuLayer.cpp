#include "GameMenuLayer.h"
#include "KeyBoardListenerManager.h"
#include "StartLayer.h"

using namespace cocos2d;


void Restart()
{
	Director::getInstance()->replaceScene(StartLayer::createScene());
}
void Resume(Layer* menu)
{
	menu->removeFromParentAndCleanup(true);
}
void BackToMainMenu()
{

}
void Exit()
{
	exit(0);
}
bool GameOverMenuLayer::init()
{
	if (!Layer::init())
		return false;
	buttons = new Button[buttoncount];
	backGround = LayerColor::create();
	backGround->setColor(Color3B(0, 0, 0));
	backGround->setOpacity(150);
	backGround->retain();
	std::string font = "Arial";
	title = Label::create("GameOver", font, 200);
	title->retain();
	buttons[0].setLabel(Label::create("Restart", font, 200));
	buttons[1].setLabel(Label::create("Back to Main Menu", font, 200));
	buttons[2].setLabel(Label::create("Exit Game", font, 200));
	Button::FUNCTYPE temp = Restart;
	//auto temp = std::bind(&Restart);
	buttons[0].setFunc(temp);
	temp = std::bind(Resume, this);
	buttons[1].setFunc(temp);
	temp = Exit;
	buttons[2].setFunc(temp);
	_selectedindex = 0;
	this->addChild(backGround, BACKGROUNDLAYER);
	this->addChild(title, TITLELAYER);
	for (int i = 0; i < 3;i++)
	{
		this->addChild(buttons[i].getLabel(), BUTTONLAYER);
	}
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameOverMenuLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//KeyBoardListenerManager::Push(listener);
	return true;
}
void GameOverMenuLayer::reSize()
{
	auto parent = this->getParent();
	if (parent != 0)
	{
		reSize(parent->getContentSize());
	}
	else
	{
		reSize(Director::getInstance()->getVisibleSize());
	}
	Select(_selectedindex);
}
void GameOverMenuLayer::reSize(const Size& size)
{
	this->setContentSize(size);
	this->backGround->setContentSize(size);
	int height = size.height;
	int hwidth = size.width/2;
	float ssc = 1.5;
	float each = ((float)height) / (buttoncount + 1 + ssc*(title == 0 ? 0 : 1));
	float heach = each / 2;
	float feach = each*0.6;
	if (title != 0)
	{
		title->setPosition(Point(hwidth, height - ssc*heach-heach));
		title->setScale(2 * feach / title->getContentSize().height);
	}
	for (int i = 0; i < buttoncount; i++)
	{
		buttons[i].getLabel()->setPosition(Point(hwidth, (buttoncount - i)*each));
		buttons[i].getLabel()->setScale(feach / buttons[i].getLabel()->getContentSize().height);
	}
}
void GameOverMenuLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	event->stopPropagation();
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		Select((_selectedindex + buttoncount - 1) % buttoncount);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		Select((_selectedindex + 1) % buttoncount);
		break;
	case EventKeyboard::KeyCode::KEY_KP_ENTER:
		Access(_selectedindex);
		break;
	default:
		break;
	}
}
void GameOverMenuLayer::onEnterTransitionDidFinish()
{
	reSize();
	Layer::onEnter();
}
GameOverMenuLayer::~GameOverMenuLayer()
{
	//KeyBoardListenerManager::Pop();

	_eventDispatcher->removeEventListenersForTarget(this);
	if (buttons != 0)
		delete[] buttons;
	if (backGround != 0)
		backGround->release();
	if (title != 0)
		title->release();
}
void GameOverMenuLayer::Select(int index)
{
	if (_selectedindex < buttoncount&&_selectedindex >= 0 && index != _selectedindex)
	{
		buttons[_selectedindex].getLabel()->setScale(originscale);
		buttons[_selectedindex].getLabel()->setTextColor(origincolor);
	}
	originscale = buttons[index].getLabel()->getScale();
	origincolor = buttons[index].getLabel()->getTextColor();
	buttons[index].getLabel()->setScale(originscale*1.1);
	buttons[index].getLabel()->setTextColor(Color4B(255, 148, 110, 255));
	_selectedindex = index;
}
void GameOverMenuLayer::Access(int index)
{
	if (index >= 0 && index < buttoncount)
	{
		(buttons[index].getFunc())();
	}
}
