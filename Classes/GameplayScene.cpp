#include "GameplayScene.h"
#include "GameoverScene.h"
#include "Game/SceneDTO.h"

USING_NS_CC;

static const int blockSize = 50;
static const Vec2 block[]{ Vec2(0, 0), Vec2(0, blockSize), Vec2(blockSize, blockSize), Vec2(blockSize, 0) };
static const Vec2 stripe[]{ Vec2(0, blockSize - 30), Vec2(0, blockSize - 20), Vec2(blockSize, blockSize - 20), Vec2(blockSize, blockSize - 30) };
static const Vec2 center{ blockSize/2, blockSize / 2 };

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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto fontSize = 24.0f;
    auto xMargin = visibleSize.width / 64;
    auto yMargin = visibleSize.height / 64;

    this->score = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", fontSize);
    this->score->setAnchorPoint(Vec2(0, 0));
    this->score->setHorizontalAlignment(TextHAlignment::LEFT);
    this->score->setVerticalAlignment(TextVAlignment::BOTTOM);
    this->score->setPosition(Vec2(origin.x + xMargin,
        origin.y + visibleSize.height - this->score->getContentSize().height - yMargin));
    this->addChild(this->score, 1);

    this->timeLeft = 120;
    this->timer = Label::createWithTTF("Time: " + std::to_string(this->timeLeft), "fonts/Marker Felt.ttf", fontSize);
    this->timer->setAnchorPoint(Vec2(0, 0));
    this->timer->setPosition(Vec2(origin.x + visibleSize.width - this->timer->getContentSize().width - xMargin,
        origin.y + visibleSize.height - this->timer->getContentSize().height - yMargin));
    this->schedule(CC_SCHEDULE_SELECTOR(GameplayScene::updateTimer), 1.0f);
    this->addChild(this->timer, 1);

    auto boardOriginX = visibleSize.width / 2 - (blockSize * Game::WIDTH + 0.2f * blockSize * (Game::WIDTH - 1)) / 2;
    auto boardOriginY = visibleSize.height / 2 - (blockSize * Game::HEIGHT + 0.2f * blockSize * (Game::HEIGHT - 1)) / 2;
    this->createBoard(boardOriginX, boardOriginY);
    SceneDTO dto = this->game.start();
    this->drawBoard(dto.boardBlocks);

    return true;
}

void GameplayScene::updateTimer(float dt)
{
    if (--this->timeLeft <= 0)
    {
        Director::getInstance()->replaceScene(GameoverScene::createScene(this->game.getScore()));
        return;
    }

    auto timeLeftStr = std::to_string(this->timeLeft);
    auto timeWidth = timeLeftStr.length();
    this->timer->setString("Time: " + timeLeftStr);
    static auto prevTimeWidth = timeWidth;
    if (prevTimeWidth != timeWidth)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        auto xMargin = visibleSize.width / 64;
        auto yMargin = visibleSize.height / 64;
        this->timer->setPosition(Vec2(origin.x + visibleSize.width - this->timer->getContentSize().width - xMargin,
            origin.y + visibleSize.height - this->timer->getContentSize().height - yMargin));
        prevTimeWidth = timeWidth;
    }
}

void GameplayScene::updateScore(int score)
{
    this->score->setString("Score: " + std::to_string(score));
}

void GameplayScene::createBoard(float originX, float originY)
{
    for (int y = 0; y < Game::HEIGHT; y++)
    {
        for (int x = 0; x < Game::WIDTH; x++)
        {
            int slotId = y * Game::WIDTH + x;
            this->createBoardSlot(slotId, originX + 1.2f * blockSize * x, originY + 1.2f * blockSize * y);
        }
    }
}

void GameplayScene::createBoardSlot(int slotId, float posX, float posY)
{
    auto node = DrawNode::create();
    node->setPosition(posX, posY);
    node->setContentSize(Size(blockSize, blockSize));
    node->setUserData((void*)slotId);
    node->setUserObject(this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        auto target = event->getCurrentTarget();
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        if (rect.containsPoint(locationInNode))
        {
            return true;
        }
        return false;
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto target = static_cast<DrawNode*>(event->getCurrentTarget());
        auto slotId = (int)target->getUserData();
        (static_cast<GameplayScene*>(target->getUserObject()))->handleMove(slotId);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, node);

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
    static const Color4F colors[]{ Color4F::RED, Color4F::BLUE, Color4F::GREEN, Color4F::YELLOW,
       Color4F::MAGENTA, Color4F::ORANGE };

    if (blockTypeId >= BlockType::RED)
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawPolygon(block, 4, colors[blockTypeId], 1, colors[blockTypeId]);
    else if (blockTypeId == BlockType::STRIPE)
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawPolygon(stripe, 4, Color4F::WHITE, 1, Color4F::WHITE);
    else if (blockTypeId == BlockType::BOMB)
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawDot(center, blockSize / 3, Color4F::WHITE);
}

void GameplayScene::destroyBlocks(const std::vector<Position>& positions)
{
    for (const auto& pos : positions)
    {
        auto slotId = pos.yPos * Game::WIDTH + pos.xPos;
        static_cast<DrawNode*>(this->boardSlots[slotId])->drawPolygon(block, 4, Color4F::BLACK, 1, Color4F::BLACK);
    }
}

void GameplayScene::handleMove(int slotId)
{
    Move move(slotId % Game::WIDTH,  slotId / Game::WIDTH);
    SceneDTO dto = this->game.makeMove(move);

    if (dto.blocksToDestroy.size() < 2) {
        if (!this->game.hasLegalMoves())
            Director::getInstance()->replaceScene(GameoverScene::createScene(this->game.getScore()));
        return;
    }

    CallFunc* destroyBlocks = CallFunc::create([=]() {
        this->destroyBlocks(dto.blocksToDestroy);
        this->updateScore(dto.score);
        this->timeLeft += dto.timeIncrement; });

    CallFunc* drawBlocks = CallFunc::create([=]() {
        this->drawBoard(dto.boardBlocks); });

    this->runAction(Sequence::create(destroyBlocks, DelayTime::create(0.05f), drawBlocks, nullptr));
}
