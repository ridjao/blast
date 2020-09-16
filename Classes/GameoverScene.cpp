#include "GameoverScene.h"
#include "GameplayScene.h"
#include "HomeScene.h"

USING_NS_CC;

int GameoverScene::score = 0;

Scene* GameoverScene::createScene(int score)
{
    GameoverScene::score = score;
    return GameoverScene::create();
}

bool GameoverScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto fontSize = 24.0f;
    auto xMargin = visibleSize.width / 64;
    auto yMargin = visibleSize.height / 64;

    auto scoreLabel = Label::createWithTTF("Score: " + std::to_string(GameoverScene::score), "fonts/Marker Felt.ttf", fontSize);
    scoreLabel->setAnchorPoint(Vec2(0, 0));
    scoreLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    scoreLabel->setVerticalAlignment(TextVAlignment::BOTTOM);
    scoreLabel->setPosition(Vec2(origin.x + xMargin,
        origin.y + visibleSize.height - scoreLabel->getContentSize().height - yMargin));
    this->addChild(scoreLabel, 1);

    UserDefault* def = UserDefault::getInstance();
    auto highscore = def->getIntegerForKey("Highscore");
    if (GameoverScene::score > highscore) {
        highscore = GameoverScene::score;
        def->setIntegerForKey("Highscore", GameoverScene::score);
    }
    auto highscoreLabel = Label::createWithTTF("Highscore: " + std::to_string(highscore), "fonts/Marker Felt.ttf", fontSize);
    highscoreLabel->setAnchorPoint(Vec2(0, 0));
    highscoreLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    highscoreLabel->setVerticalAlignment(TextVAlignment::BOTTOM);
    highscoreLabel->setPosition(Vec2(origin.x + xMargin,
        origin.y + visibleSize.height - scoreLabel->getContentSize().height - highscoreLabel->getContentSize().height - yMargin));
    this->addChild(highscoreLabel, 1);

    auto gameoverLabel = Label::createWithTTF("GAMEOVER", "fonts/Marker Felt.ttf", fontSize + 4);
    gameoverLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2*visibleSize.height / 3));
    this->addChild(gameoverLabel, 1);

    auto startLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", fontSize);
    auto startMenuItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(GameoverScene::menuStartCallback, this));
    auto startMenu = Menu::create(startMenuItem, NULL);
    startMenu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(startMenu, 1);

    auto homeLabel = Label::createWithTTF("Home", "fonts/Marker Felt.ttf", fontSize);
    auto homeMenuItem = MenuItemLabel::create(homeLabel, CC_CALLBACK_1(GameoverScene::menuHomeCallback, this));
    auto homeMenu = Menu::create(homeMenuItem, NULL);
    homeMenu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + startLabel->getContentSize().height));
    this->addChild(homeMenu, 1);

    return true;
}

void GameoverScene::menuStartCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameplayScene::createScene());
}

void GameoverScene::menuHomeCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(HomeScene::createScene());
}
