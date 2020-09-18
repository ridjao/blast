#pragma once
#include "cocos2d.h"
#include "Game/Position.h"
#include "Game/Game.h"
#include "Game/BlockTypeIds.h"

class GameplayScene : public cocos2d::Scene
{
    int timeLeft;
    cocos2d::Label* timer;
    cocos2d::Label* score;
    Game game;
    std::vector<cocos2d::Node*> boardSlots;

    void updateTimer(float dt);
    void updateScore(int score);
    void createBoard(float originX, float originY);
    void createBoardSlot(int slotId, float posX, float posY);
    void drawBoard(const std::vector<BlockType>& blocks);
    void drawBlock(int slotId, int color);
    void handleMove(int slotId);
    void destroyBlocks(const std::vector<Position>& positions);
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameplayScene);
};

