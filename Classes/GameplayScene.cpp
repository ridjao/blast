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

    this->score = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
    this->score->setAnchorPoint(Vec2(0, 0));
    this->score->setHorizontalAlignment(TextHAlignment::LEFT);
    this->score->setVerticalAlignment(TextVAlignment::BOTTOM);
    this->score->setPosition(Vec2(origin.x + 5,
        origin.y + visibleSize.height - this->score->getContentSize().height - 5));
    this->addChild(this->score, 1);

    this->timeLeft = 120;
    this->timer = Label::createWithTTF("Time: " + std::to_string(this->timeLeft), "fonts/Marker Felt.ttf", 24);
    this->timer->setPosition(Vec2(origin.x + visibleSize.width - visibleSize.width / 8,
        origin.y + visibleSize.height - this->timer->getContentSize().height));
    this->schedule(CC_SCHEDULE_SELECTOR(GameplayScene::updateTimer), 1.0f);
    this->addChild(this->timer, 1);

    this->createBoard(275, 150);
    SceneDTO dto = this->game.start();
    this->drawBoard(dto.boardBlocks);

    return true;
}

void GameplayScene::updateTimer(float dt)
{
    if (this->timeLeft == 0)
    {
        Director::getInstance()->replaceScene(GameoverScene::createScene(this->game.getScore()));
        return;
    }
    this->timer->setString("Time: " + std::to_string(--this->timeLeft));
}

void GameplayScene::updateScore(int score)
{
    this->score->setString(std::string("Score: ") + std::to_string(score));
}

void GameplayScene::createBoard(int originX, int originY)
{
    for (int y = 0; y < Game::HEIGHT; y++)
    {
        for (int x = 0; x < Game::WIDTH; x++)
        {
            int blockId = y * Game::WIDTH + x;
            this->createBoardSlot(blockId, originX + (blockSize + 10.f) * x, originY + (blockSize + 10.f) * y);
        }
    }
}

void GameplayScene::createBoardSlot(int blockId, float posX, float posY)
{
    auto node = DrawNode::create();
    node->setPosition(posX, posY);
    node->setContentSize(Size(blockSize, blockSize));
    node->setUserData((void*)blockId);
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
        auto blockId = (int)target->getUserData();
        (static_cast<GameplayScene*>(target->getUserObject()))->handleMove(blockId);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, node);

    this->addChild(node);
    this->board.push_back(node);
}

void GameplayScene::drawBoard(const std::vector<BlockTypeId>& blocks)
{
    for (size_t blockId = 0; blockId < blocks.size(); blockId++)
    {
        BlockTypeId blockTypeId = blocks[blockId];
        this->drawBlock(blockId, blockTypeId);
    }
}

void GameplayScene::drawBlock(int blockId, int blockTypeId)
{
    static const Color4F colors[]{ Color4F::RED, Color4F::BLUE, Color4F::GREEN, Color4F::YELLOW,
       Color4F::MAGENTA, Color4F::ORANGE };

    if (blockTypeId >= BlockTypeId::RED)
        static_cast<DrawNode*>(this->board[blockId])->drawPolygon(block, 4, colors[blockTypeId], 1, colors[blockTypeId]);
    else if (blockTypeId == BlockTypeId::STRIPE)
        static_cast<DrawNode*>(this->board[blockId])->drawPolygon(stripe, 4, Color4F::WHITE, 1, Color4F::WHITE);
    else if (blockTypeId == BlockTypeId::BOMB)
        static_cast<DrawNode*>(this->board[blockId])->drawDot(center, blockSize / 3, Color4F::WHITE);
}

void GameplayScene::destroyBlocks(const std::vector<Position>& positions)
{
    for (const auto& pos : positions)
    {
        auto blockId = pos.yPos * Game::WIDTH + pos.xPos;
        static_cast<DrawNode*>(this->board[blockId])->drawPolygon(block, 4, Color4F::BLACK, 1, Color4F::BLACK);
    }
}

void GameplayScene::handleMove(int blockId)
{
    Move move(blockId % Game::WIDTH,  blockId / Game::WIDTH);
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
