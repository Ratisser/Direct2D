#include "PreCompile.h"
#include "GameEngineFSM.h"

#include "GameEngineDebug.h"

GameEngineFSM::GameEngineFSM()
	: currentState_(nullptr)
	, nextState_(nullptr)
{

}

GameEngineFSM::~GameEngineFSM()
{
	for (const std::pair<std::string, GameEngineFSM::State*>& pair : allState_)
	{
		delete pair.second;
	}
}


void GameEngineFSM::CreateState(const std::string& _stateName, std::function<void(float)> _start, std::function<void(float)> _update, std::function<void(float)> _end)
{
	std::map<std::string, GameEngineFSM::State*>::iterator findIter = allState_.find(_stateName);

	if (findIter != allState_.end())
	{
		GameEngineDebug::MsgBoxError("state named : [" + _stateName + "] already exists");
	}

	GameEngineFSM::State* newState = new GameEngineFSM::State(_stateName, _start, _update, _end);

	allState_.insert(std::pair<std::string, GameEngineFSM::State*>(_stateName, newState));
}

void GameEngineFSM::ChangeState(const std::string& _stateName)
{
	std::map<std::string, GameEngineFSM::State*>::iterator findIter = allState_.find(_stateName);

	if (findIter == allState_.end())
	{
		GameEngineDebug::MsgBoxError("state named : [" + _stateName + "] no exists");
	}

	nextState_ = findIter->second;
}

void GameEngineFSM::Update(float _deltaTime)
{
	if (nullptr != nextState_)
	{
		if (nullptr != currentState_ && nullptr != currentState_->end_)
		{
			currentState_->end_(_deltaTime);
		}
		nextState_->start_(_deltaTime);
		currentState_ = nextState_;
		nextState_ = nullptr;
	}

	if (nullptr != currentState_)
	{
		currentState_->update_(_deltaTime);
	}
}

GameEngineFSM::State::State(const std::string& _stateName, std::function<void(float)> _start, std::function<void(float)> _update, std::function<void(float)> _end)
	: stateName_(_stateName)
	, start_(_start)
	, update_(_update)
	, end_(_end)
{
}


std::string GameEngineFSM::GetCurrentStateName()
{
	if (nullptr == currentState_)
	{
		return std::string();
	}
	else
	{
		return currentState_->stateName_;
	}
}