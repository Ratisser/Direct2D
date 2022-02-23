#include "PreCompile.h"
#include "GameEngineInput.h"
#include <GameEngineBase/GameEngineDebug.h>

GameEngineInput* GameEngineInput::instance_ = new GameEngineInput();

GameEngineInput::GameEngineInput()
{
}

GameEngineInput::~GameEngineInput()
{
	std::map<std::string, GameEngineKey*>::iterator startIter = keys_.begin();
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

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
	std::map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

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
	std::map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->down_;
}

bool GameEngineInput::IsKeyUp(const std::string& _keyName)
{
	std::map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->up_;
}

bool GameEngineInput::IsKeyPress(const std::string& _keyName)
{
	std::map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->press_;
}

bool GameEngineInput::IsKeyFree(const std::string& _keyName)
{
	std::map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		GameEngineDebug::AssertFalse();
		return false;
	}

	return findIter->second->free_;
}

bool GameEngineInput::IsKeyAvailable(const std::string& _keyName)
{
	std::map<std::string, GameEngineKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		return false;
	}

	return true;
}

void GameEngineInput::update()
{
	std::map<std::string, GameEngineKey*>::iterator startIter = keys_.begin();
	std::map<std::string, GameEngineKey*>::iterator endIter = keys_.end();

	while (startIter != endIter)
	{
		startIter->second->update();
		startIter++;
	}
}
