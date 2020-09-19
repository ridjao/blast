#pragma once
#include "cocos2d.h"
#include "Game/Position.h"
#include "Game/Game.h"
#include "Game/BlockTypeIds.h"

class GameplayScene : public cocos2d::Scene
{
    Game game;
    std::vector<cocos2d::Node*> boardSlots;
    cocos2d::Label* timer;
    cocos2d::Label* score;
    cocos2d::Label* hits;
    int timeLeft;

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

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameplayScene);
};

