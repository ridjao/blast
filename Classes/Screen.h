#pragma once
#include "cocos2d.h"
#include "Game/Game.h"

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

    static inline const float labelFontSize = 24.0f;

    static Screen& getInstance()
    {
        static Screen instance;
        return instance;
    }
private:
    Screen() :
        visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() },
        origin{ cocos2d::Director::getInstance()->getVisibleOrigin() },
        xMargin{ visibleSize.width / 64 },
        yMargin{ 3 * visibleSize.height / 64 },
        blockSize{ (visibleSize.width < visibleSize.height / 2) ? (visibleSize.width - 2 * xMargin) / (Game::WIDTH + 0.2f * (Game::WIDTH - 1)) :
                        blockSize = (visibleSize.height / 2 - 2 * yMargin) / (Game::HEIGHT + 0.2f * (Game::HEIGHT - 1)) },
        block{ cocos2d::Vec2(0, 0), cocos2d::Vec2(0, blockSize), cocos2d::Vec2(blockSize, blockSize), cocos2d::Vec2(blockSize, 0) },
        stripe{
            cocos2d::Vec2(0, blockSize / 2 - blockSize / 8), cocos2d::Vec2(0, blockSize / 2 + blockSize / 8),
            cocos2d::Vec2(blockSize, blockSize / 2 + blockSize / 8), cocos2d::Vec2(blockSize, blockSize / 2 - blockSize / 8)
        },
        center{ blockSize * 1.f / 2, blockSize * 1.f / 2 } {}
};
