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
    std::vector<cocos2d::Node*> board;

    void updateTimer(float dt);
    void updateScore(int score);
    void createBoard(int originX, int originY);
    void createBoardSlot(int blockId, float posX, float posY);
    void drawBoard(const std::vector<BlockTypeId>& blocks);
    void drawBlock(int blockId, int color);
    void handleMove(int blockId);
    void destroyBlocks(const std::vector<Position>& positions);
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameplayScene);
};

