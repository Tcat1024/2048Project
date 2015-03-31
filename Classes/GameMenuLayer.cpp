#include "GameMenuLayer.h"
#include "GameLayer.h"

using namespace cocos2d;
void Newgame()
{
	auto scene = GameScene::create();
	Director::getInstance()->replaceScene(scene);
	scene->mainLayer->StartNewGame();
}
void Restart(Layer* menu)
{
	((GameLayer*)menu->getParent())->StartNewGame();
	menu->removeFromParentAndCleanup(true);
}
void Resume(Layer* menu)
{
	menu->removeFromParentAndCleanup(true);
}
void MainMenu(Layer *menu)
{
	((GameLayer*)menu->getParent())->SaveGame();
	Director::getInstance()->replaceScene(MainMenuLayer::createScene());
}
void Exit()
{
	exit(0);
}
void Continue()
{
	auto scene = GameScene::create();
	Director::getInstance()->replaceScene(scene);
	scene->mainLayer->LoadGame();
}
bool GamePausedMenuLayer::init()
{
	if (!Layer::init())
		return false;
	buttoncount = 4;
	buttons = new Button[buttoncount];
	backGround = LayerColor::create();
	backGround->setColor(Color3B(0, 0, 0));
	backGround->setOpacity(150);
	backGround->retain();
	std::string font = "Arial";
	title = Label::create("Paused", font, 200);
	title->retain();
	buttons[0].setLabel(Label::create("Resume", font, 200));
	buttons[1].setLabel(Label::create("Restart", font, 200));
	buttons[2].setLabel(Label::create("Main Menu", font, 200));
	buttons[3].setLabel(Label::create("Exit Game", font, 200));
	Button::FUNCTYPE temp = std::bind(Resume,this);
	//auto temp = std::bind(&Restart);
	buttons[0].setFunc(temp);
	temp = std::bind(Restart, this);
	buttons[1].setFunc(temp);
	temp = std::bind(MainMenu, this);
	buttons[2].setFunc(temp);	
	temp = Exit;
	buttons[3].setFunc(temp);
	_selectedindex = 0;
	this->addChild(backGround, BACKGROUNDLAYER);
	this->addChild(title, TITLELAYER);
	for (int i = 0; i < buttoncount; i++)
	{
		this->addChild(buttons[i].getLabel(), BUTTONLAYER);
	}
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GamePausedMenuLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//KeyBoardListenerManager::Push(listener);
	return true;
}
bool GameOverMenuLayer::init()
{
	if (!Layer::init())
		return false;
	buttoncount = 3;
	buttons = new Button[buttoncount];
	backGround = LayerColor::create();
	backGround->setColor(Color3B(0, 0, 0));
	backGround->setOpacity(150);
	backGround->retain();
	std::string font = "Arial";
	title = Label::create("GameOver", font, 200);
	title->retain();
	buttons[0].setLabel(Label::create("Restart", font, 200));
	buttons[1].setLabel(Label::create("Main Menu", font, 200));
	buttons[2].setLabel(Label::create("Exit Game", font, 200));
	Button::FUNCTYPE temp = std::bind(Restart, this);
	//auto temp = std::bind(&Restart);
	buttons[0].setFunc(temp);
	temp = std::bind(MainMenu, this);
	buttons[1].setFunc(temp);
	temp = Exit;
	buttons[2].setFunc(temp);
	_selectedindex = 0;
	this->addChild(backGround, BACKGROUNDLAYER);
	this->addChild(title, TITLELAYER);
	for (int i = 0; i < buttoncount; i++)
	{
		this->addChild(buttons[i].getLabel(), BUTTONLAYER);
	}
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameOverMenuLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//KeyBoardListenerManager::Push(listener);
	return true;
}
void MenuLayer::reSize()
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
void MenuLayer::reSize(const Size& size)
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
void MenuLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	event->stopPropagation();
	int temp = _selectedindex;
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		while ((temp = (temp + buttoncount - 1) % buttoncount) != _selectedindex&&!buttons[temp].Enable);
		if (temp!=_selectedindex)
			Select(temp);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		while ((temp = (temp + 1) % buttoncount) != _selectedindex&&!buttons[temp].Enable);
		if (temp != _selectedindex)
			Select(temp);
		break;
	case EventKeyboard::KeyCode::KEY_KP_ENTER:
		Access(temp);
		break;
	default:
		break;
	}
}
void MenuLayer::onEnterTransitionDidFinish()
{
	reSize();
	Layer::onEnterTransitionDidFinish();
}
MenuLayer::~MenuLayer()
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
void MenuLayer::Select(const int &index)
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
void MenuLayer::Access(const int &index)
{
	if (index >= 0 && index < buttoncount)
	{
		buttons[index].Clicked();
	}
}
Scene* MainMenuLayer::createScene()
{
	auto *scene = Scene::create();
	auto *layer = MainMenuLayer::create();
	scene->addChild(layer);
	return scene;
}
bool MainMenuLayer::init()
{
	if (!Layer::init())
		return false;
	buttoncount = 4;
	buttons = new Button[buttoncount];
	backGround = LayerColor::create();
	backGround->setColor(Color3B(0, 0, 0));
	backGround->retain();
	std::string font = "Arial";
	title = Label::create("2048", font, 200);
	title->retain();
	buttons[0].setLabel(Label::create("New Game", font, 200));
	buttons[1].setLabel(Label::create("Continue", font, 200));
	if (!GameLayer::CheckSaveData())
	{
		buttons[1].getLabel()->setTextColor(Color4B(140, 140, 140, 255));
		buttons[1].Enable = false;
	}
	buttons[2].setLabel(Label::create("Score List", font, 200));	
	buttons[2].getLabel()->setTextColor(Color4B(140, 140, 140, 255));
	buttons[2].Enable = false;
	buttons[3].setLabel(Label::create("Exit Game", font, 200));
	Button::FUNCTYPE temp = Newgame;
	//auto temp = std::bind(&Restart);
	buttons[0].setFunc(temp);
	temp = Continue;
	buttons[1].setFunc(temp);
	//temp = MainMenu;
	//buttons[2].setFunc(temp);
	temp = Exit;
	buttons[3].setFunc(temp);
	_selectedindex = 0;
	this->addChild(backGround, BACKGROUNDLAYER);
	this->addChild(title, TITLELAYER);
	for (int i = 0; i < buttoncount; i++)
	{
		this->addChild(buttons[i].getLabel(), BUTTONLAYER);
	}
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainMenuLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//KeyBoardListenerManager::Push(listener);
	return true;
}
