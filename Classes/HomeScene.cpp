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
    auto fontSize = 24.0f;
    auto xMargin = visibleSize.width / 64;
    auto yMargin = visibleSize.height / 64;

    UserDefault* def = UserDefault::getInstance();
    auto highscore = def->getIntegerForKey("Highscore");
    auto highscoreLabel = Label::createWithTTF("Highscore: " + std::to_string(highscore), "fonts/Marker Felt.ttf", fontSize);
    highscoreLabel->setAnchorPoint(Vec2(0, 0));
    highscoreLabel->setPosition(Vec2(origin.x + xMargin,
        origin.y + visibleSize.height - highscoreLabel->getContentSize().height - yMargin));
    this->addChild(highscoreLabel, 1);

    auto startLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", fontSize);
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
