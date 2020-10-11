#include <chrono>
#include "GameplayScene.h"
#include "GameoverScene.h"

USING_NS_CC;

static const std::string fontFilePath = "fonts/Marker Felt.ttf";

static const int INITIAL_TIME = 120;
static const int REFRESH_TICKS = 60;

static auto timeHitsSet = std::chrono::steady_clock::now();
static auto gameover = false;

static std::mutex sceneLock;

int GameplayScene::timeLeft;
Game GameplayScene::game;

Scene* GameplayScene::createScene()
{
    return GameplayScene::create();
}

bool GameplayScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto boardOriginX = screen.origin.x + screen.visibleSize.width / 2 - (screen.blockSize * Game::WIDTH + 0.2f * screen.blockSize * (Game::WIDTH - 1)) / 2;
    auto boardOriginY = screen.origin.y + screen.visibleSize.height / 2 - (screen.blockSize * Game::HEIGHT + 0.2f * screen.blockSize * (Game::HEIGHT - 1)) / 2;
    this->createBoard(boardOriginX, boardOriginY);
    auto gameStarted = !GameplayScene::game.getBoardBlocks().empty();
    if (!gameStarted)
    {
        GameplayScene::timeLeft = INITIAL_TIME;
        GameplayScene::game.start();
        gameover = false;
    }
    this->drawBoard(GameplayScene::game.getBoardBlocks());

    this->scoreDisplay = Label::createWithTTF("Score: " + std::to_string(GameplayScene::game.getScore()), fontFilePath, Screen::labelFontSize);
    this->scoreDisplay->setAnchorPoint(Vec2(0, 0));
    this->scoreDisplay->setPosition(Vec2(screen.origin.x + screen.xMargin,
        screen.origin.y + screen.visibleSize.height - this->scoreDisplay->getContentSize().height - screen.yMargin));
    this->addChild(this->scoreDisplay, 1);

    this->timerDisplay = Label::createWithTTF("Time: " + std::to_string(GameplayScene::timeLeft), fontFilePath, Screen::labelFontSize);
    this->timerDisplay->setAnchorPoint(Vec2(0, 0));
    this->setTimerPosition();
    this->schedule(CC_SCHEDULE_SELECTOR(GameplayScene::runPeriodicTasks), 1.0f);
    this->addChild(this->timerDisplay, 1);

    this->hitsDisplay = Label::createWithTTF("", fontFilePath, Screen::labelFontSize + 10);
    this->hitsDisplay->setAnchorPoint(Vec2(0, 0));
    this->setHitsPosition();
    this->addChild(this->hitsDisplay, 1);

    return true;
}

void GameplayScene::setTimerPosition()
{
    this->timerDisplay->setPosition(Vec2(screen.origin.x + screen.visibleSize.width - this->timerDisplay->getContentSize().width - screen.xMargin,
        screen.origin.y + screen.visibleSize.height - this->timerDisplay->getContentSize().height - screen.yMargin));
}

void GameplayScene::runPeriodicTasks(float dt)
{
    const std::lock_guard<std::mutex> lock(sceneLock);

    this->updateTimer();
    this->clearHits();

    // cocos2d-x grinds to a halt after a while of drawing and re-drawing polygons,
    // so refresh the scene after so many ticks
    static int ticks = 0;
    if (!gameover && !(++ticks % REFRESH_TICKS)) {
        Director::getInstance()->replaceScene(GameplayScene::createScene());
    }
}

void GameplayScene::updateTimer()
{
    if (--GameplayScene::timeLeft <= 0)
    {
        gameover = true;
        GameplayScene::game.end();
        Director::getInstance()->replaceScene(GameoverScene::createScene(GameplayScene::game.getScore()));
        return;
    }

    this->timerDisplay->setString("Time: " + std::to_string(this->timeLeft));
    auto timeWidth = this->timerDisplay->getContentSize().width;
    static auto prevTimeWidth = timeWidth;
    if (prevTimeWidth != timeWidth)
    {
        this->setTimerPosition();
        prevTimeWidth = timeWidth;
    }
}

void GameplayScene::updateScore(int score)
{
    this->scoreDisplay->setString("Score: " + std::to_string(score));
}

void GameplayScene::createBoard(float originX, float originY)
{
    for (int y = 0; y < Game::HEIGHT; y++)
    {
        for (int x = 0; x < Game::WIDTH; x++)
        {
            int slotId = y * Game::WIDTH + x;
            this->createBoardSlot(slotId, originX + 1.2f * screen.blockSize * x, originY + 1.2f * screen.blockSize * y);
        }
    }
}

void GameplayScene::createBoardSlot(int slotId, float posX, float posY)
{
    static int slots[Game::HEIGHT * Game::WIDTH];
    slots[slotId] = slotId;

    auto node = DrawNode::create();
    node->setAnchorPoint(Vec2(0, 0));
    node->setPosition(posX, posY);
    node->setContentSize(Size(screen.blockSize, screen.blockSize));
    node->setUserData(slots+slotId);
    node->setUserObject(this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        auto target = event->getCurrentTarget();
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect{ 0, 0, s.width, s.height };
        if (rect.containsPoint(locationInNode))
        {
            return true;
        }
        return false;
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto target = static_cast<DrawNode*>(event->getCurrentTarget());
        auto slotId = *(reinterpret_cast<int*>(target->getUserData()));
        (static_cast<GameplayScene*>(target->getUserObject()))->handleMove(slotId);
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, node);

    this->addChild(node);
    this->boardSlots.push_back(node);
}

void GameplayScene::drawBoard(const std::vector<BlockType>& blocks)
{
    for (size_t slotId = 0; slotId < blocks.size(); slotId++)
    {
        BlockType blockTypeId = blocks[slotId];
        this->drawBlock(slotId, blockTypeId);
    }
}

void GameplayScene::drawBlock(int slotId, int blockTypeId)
{
    Color4F pastelRed    (Color4B(255, 102,  99, 255));
    Color4F pastelBlue   (Color4B(158, 193, 207, 255));
    Color4F pastelGreen  (Color4B(158, 224, 158, 255));
    Color4F pastelYellow (Color4B(253, 253, 151, 255));
    Color4F pastelViolet (Color4B(204, 153, 201, 255));
    Color4F pastelOrange (Color4B(254, 177,  68, 255));

    static const Color4F colors[]{ pastelRed, pastelBlue, pastelGreen, pastelYellow, pastelViolet, pastelOrange };

    if (blockTypeId >= BlockType::RED)
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawPolygon(screen.block, 4, colors[blockTypeId], 1, colors[blockTypeId]);
    else if (blockTypeId == BlockType::STRIPE)
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawPolygon(screen.stripe, 4, Color4F::WHITE, 1, Color4F::WHITE);
    else if (blockTypeId == BlockType::BOMB)
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawDot(screen.center, screen.blockSize / 3, Color4F::WHITE);
}

void GameplayScene::setHitsPosition()
{
    auto boardEndY = screen.origin.y + screen.visibleSize.height / 2 + (screen.blockSize * Game::HEIGHT + 0.2f * screen.blockSize * (Game::HEIGHT - 1)) / 2;
    this->hitsDisplay->setPosition(Vec2(screen.origin.x + (screen.visibleSize.width - this->hitsDisplay->getContentSize().width)/2,
        boardEndY + (screen.visibleSize.height - screen.yMargin - boardEndY - 2 * this->hitsDisplay->getContentSize().height)/2));
}

void GameplayScene::displayHits(int hits)
{
    this->hitsDisplay->setString(std::to_string(hits) + " hits!");
    this->setHitsPosition();
    timeHitsSet = std::chrono::steady_clock::now();
}

void GameplayScene::clearHits()
{
    std::chrono::duration<double> elapsedSeconds = std::chrono::steady_clock::now() - timeHitsSet;
    if (elapsedSeconds.count() >= 1)
    {
        this->hitsDisplay->setString("");
        this->setHitsPosition();
    }
}

void GameplayScene::destroyBlocks(const std::vector<Position>& positions)
{
    for (const auto& pos : positions)
    {
        auto slotId = pos.yPos * Game::WIDTH + pos.xPos;
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawPolygon(screen.block, 4, Color4F::BLACK, 1, Color4F::BLACK);
    }
}

void GameplayScene::handleMove(int slotId)
{
    const std::lock_guard<std::mutex> lock(sceneLock);

    Move move(slotId % Game::WIDTH,  slotId / Game::WIDTH);
    std::vector<Position> blocksToDestroy = GameplayScene::game.makeMove(move);

    if (blocksToDestroy.size() < 2)
    {
        if (!GameplayScene::game.hasLegalMoves())
        {
            gameover = true;
            GameplayScene::game.end();
            Director::getInstance()->replaceScene(GameoverScene::createScene(GameplayScene::game.getScore()));
        }
        return;
    }

    this->destroyBlocks(blocksToDestroy);
    this->updateScore(GameplayScene::game.getScore());
    this->displayHits(blocksToDestroy.size());
    this->timeLeft += GameplayScene::game.computeTimeIncrement(blocksToDestroy.size());

    CallFunc* redrawBlocks = CallFunc::create([=]() {
        this->drawBoard(GameplayScene::game.getBoardBlocks());
        });

    this->runAction(Sequence::create(DelayTime::create(0.05f), redrawBlocks, nullptr));
}
