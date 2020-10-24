#pragma once
#include "cocos2d.h"

class GameoverScene : public cocos2d::Scene
{
    static int score;
    void menuStartCallback(Ref* pSender);
    void menuHomeCallback(Ref* pSender);
    void runPeriodicTasks(float dt);
public:
    static cocos2d::Scene* createScene(int score);
    virtual bool init();
    CREATE_FUNC(GameoverScene);
};
