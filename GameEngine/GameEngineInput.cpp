#include "PreCompile.h"
#include "GameEngineInput.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngine\GameEngineWindow.h>

GameEngineInput* GameEngineInput::instance_ = new GameEngineInput();

GameEngineInput::GameEngineInput()
	: prevMousePos_(float4::ZERO)
	, currentMousePos_(float4::ZERO)
	, mouseDirection_(float4::ZERO)
{
}

GameEngineInput::~GameEngineInput()
{
	std::unordered_map<std::string, GameEngineKey*>::iterator startIter = keys_.begin();
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	while (startIter != endIter)
	{
		delete startIter->second;
		startIter->second = nullptr;
		startIter++;
	}

	keys_.clear();
}

void GameEngineInput::GameEngineKey::update()
{
	if (0 != GetAsyncKeyState(key_))
	{
		if (false == press_)
		{
			down_ = true;
			press_ = true;
			up_ = false;
			free_ = false;
		}
		else if (true == press_)
		{
			down_ = false;
			press_ = true;
			up_ = false;
			free_ = false;
		}
	}
	else
	{
		if (true == press_)
		{
			up_ = true;
			press_ = false;
			down_ = false;
			free_ = true;
		}
		else if (false == press_)
		{
			down_ = false;
			up_ = false;
			press_ = false;
			free_ = true;
		}
	}
}

GameEngineInput::GameEngineKey::GameEngineKey()
	: key_(-1)
	, down_(false)
	, up_(false)
	, press_(false)
	, free_(true)
{
}

GameEngineInput::GameEngineKey::~GameEngineKey()
{

}

bool GameEngineInput::CreateKey(const std::string& _keyName, int _key)
{
	std::unordered_map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter != endIter)
	{
		return false;
	}

	GameEngineKey* newKey = new GameEngineKey();
	newKey->SetName(_keyName);
	newKey->key_ = _key;

	keys_[_keyName] = newKey;

	return true;
}

bool GameEngineInput::IsKeyDown(const std::string& _keyName)
{
	std::unordered_map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->down_;
}

bool GameEngineInput::IsKeyUp(const std::string& _keyName)
{
	std::unordered_map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->up_;
}

bool GameEngineInput::IsKeyPress(const std::string& _keyName)
{
	std::unordered_map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->press_;
}

bool GameEngineInput::IsKeyFree(const std::string& _keyName)
{
	std::unordered_map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->free_;
}

bool GameEngineInput::IsKeyAvailable(const std::string& _keyName)
{
	std::unordered_map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		return false;
	}

	return true;
}

float4 GameEngineInput::GetMouseDirection()
{
	return mouseDirection_;
}

void GameEngineInput::update()
{
	std::unordered_map<std::string, GameEngineKey*>::iterator startIter = keys_.begin();
	std::unordered_map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	while (startIter != endIter)
	{
		startIter->second->update();
		startIter++;
	}

	POINT mousePoint = { 0, };
	GetCursorPos(&mousePoint);
	::ScreenToClient(GameEngineWindow::GetInst().GetWindowHWND(), &mousePoint);

	float4 windowSize = GameEngineWindow::GetInst().GetSize();
	
	mousePoint.x -= windowSize.ihx();
	mousePoint.y -= windowSize.ihy();

	currentMousePos_.x = static_cast<float>(mousePoint.x);
	currentMousePos_.y = -static_cast<float>(mousePoint.y);

	mouseDirection_ = currentMousePos_ - prevMousePos_;

	prevMousePos_ = currentMousePos_;
}
