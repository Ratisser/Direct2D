#include "PreCompile.h"
#include "GameEngineFSM.h"

GameEngineFSM::GameEngineFSM()
	: current_(nullptr)
	, next_(nullptr)
{

}

GameEngineFSM::~GameEngineFSM()
{

}


void GameEngineFSM::CreateState(const std::string& _stateName, std::function<void()> _start, std::function<void()> _update, std::function<void()> _init, std::function<void()> _end)
{
}

void GameEngineFSM::Update()
{
	if (nullptr != next_)
	{
		if (nullptr != current_)
		{
			current_->end_();
		}

	}
}
