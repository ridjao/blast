#include "FirebaseHelper.h"
#include "GameplayScene.h"
#include "HomeScene.h"
#include "Screen.h"

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

    auto visibleSize = Screen::getInstance().visibleSize;
    Vec2 origin = Screen::getInstance().origin;
    auto fontSize = Screen::labelFontSize;
    auto xMargin = Screen::getInstance().xMargin;
    auto yMargin = Screen::getInstance().yMargin;

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

    this->schedule(CC_SCHEDULE_SELECTOR(HomeScene::runPeriodicTasks), 1.0f);
    return true;
}

void HomeScene::menuStartCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameplayScene::createScene());
}

void HomeScene::runPeriodicTasks(float dt)
{
    FirebaseHelper::showBannerAd();
}
