#pragma once
#include "cocos2d.h"

class HomeScene : public cocos2d::Scene
{
    void menuStartCallback(cocos2d::Ref* pSender);
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HomeScene);
};
