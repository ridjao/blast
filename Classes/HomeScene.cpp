#include "HomeScene.h"
#include "GameplayScene.h"

USING_NS_CC;

Scene* HomeScene::createScene()
{
    return HomeScene::create();
}

bool HomeScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    UserDefault* def = UserDefault::getInstance();
    auto highscore = def->getIntegerForKey("Highscore");
    auto highscoreLabel = Label::createWithTTF("Highscore: " + std::to_string(highscore), "fonts/Marker Felt.ttf", 24);
    highscoreLabel->setAnchorPoint(Vec2(0, 0));
    highscoreLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    highscoreLabel->setVerticalAlignment(TextVAlignment::BOTTOM);
    highscoreLabel->setPosition(Vec2(origin.x + 5,
        origin.y + visibleSize.height - highscoreLabel->getContentSize().height - 5));
    this->addChild(highscoreLabel, 1);

    auto startLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 24);
    auto startMenuItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(HomeScene::menuStartCallback, this));
    auto startMenu = Menu::create(startMenuItem, NULL);
    startMenu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(startMenu, 1);

    return true;
}

void HomeScene::menuStartCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameplayScene::createScene());
}
