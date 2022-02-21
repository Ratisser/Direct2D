#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor()
	: transform_(std::make_unique<GameEngineTransform>())
	, level_(nullptr)
{

}

GameEngineActor::~GameEngineActor()
{

}

GameEngineLevel* GameEngineActor::GetLevel()
{
	return level_;
}

void GameEngineActor::SetLevel(GameEngineLevel* _level)
{
	level_ = _level;
}
