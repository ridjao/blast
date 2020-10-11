#pragma once
#include "cocos2d.h"
#include "Game/Position.h"
#include "Game/Game.h"
#include "Game/BlockTypeIds.h"

class GameplayScene : public cocos2d::Scene
{
    static Game game;
    static int timeLeft;

    std::vector<cocos2d::Node*> boardSlots;
    cocos2d::Label* timerDisplay;
    cocos2d::Label* scoreDisplay;
    cocos2d::Label* hitsDisplay;    

    void runPeriodicTasks(float dt);
    void updateTimer();
    void updateScore(int score);
    void createBoard(float originX, float originY);
    void createBoardSlot(int slotId, float posX, float posY);
    void drawBoard(const std::vector<BlockType>& blocks);
    void drawBlock(int slotId, int color);
    void handleMove(int slotId);
    void destroyBlocks(const std::vector<Position>& positions);
    void setTimerPosition();
    void setHitsPosition();
    void displayHits(int hits);
    void clearHits();

    struct Screen
    {
        cocos2d::Size visibleSize;
        cocos2d::Vec2 origin;
        float xMargin;
        float yMargin;
        float blockSize;
        cocos2d::Vec2 block[4];
        cocos2d::Vec2 stripe[4];
        cocos2d::Vec2 center{ blockSize * 1.f / 2, blockSize * 1.f / 2 };

        Screen() :
            visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() },
            origin{ cocos2d::Director::getInstance()->getVisibleOrigin() },
            xMargin{ visibleSize.width / 64 },
            yMargin{ visibleSize.height / 64 },
            blockSize{(visibleSize.width < visibleSize.height / 2) ? (visibleSize.width - 2 * xMargin) / (Game::WIDTH + 0.2f * (Game::WIDTH - 1)) : 
                            blockSize = (visibleSize.height / 2 - 2 * yMargin) / (Game::HEIGHT + 0.2f * (Game::HEIGHT - 1))},
            block{ cocos2d::Vec2(0, 0), cocos2d::Vec2(0, blockSize), cocos2d::Vec2(blockSize, blockSize), cocos2d::Vec2(blockSize, 0) },
            stripe{
                cocos2d::Vec2(0, blockSize / 2 - blockSize / 8), cocos2d::Vec2(0, blockSize / 2 + blockSize / 8),
                cocos2d::Vec2(blockSize, blockSize / 2 + blockSize / 8), cocos2d::Vec2(blockSize, blockSize / 2 - blockSize / 8)
            },
            center{ blockSize * 1.f / 2, blockSize * 1.f / 2 } {}
    } screen;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameplayScene);
};

