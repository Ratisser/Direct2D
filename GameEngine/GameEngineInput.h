#pragma once

#include <GameEngineBase/GameEngineObjectNameBase.h>

class GameEngineCore;
class GameEngineInput : public GameEngineObjectNameBase
{
	friend GameEngineCore;
private:
	class GameEngineKey : public GameEngineObjectNameBase
	{
		friend GameEngineInput;
	private:
		GameEngineKey();
		~GameEngineKey();

		void update();

	private:
		int key_;
		bool down_;
		bool up_;
		bool press_;
		bool free_;
	};

public:
	static GameEngineInput& GetInstance()
	{
		return *instance_;
	}

	static void Destroy()
	{
		if (nullptr != instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

public:
	~GameEngineInput();

	GameEngineInput(const GameEngineInput& _other) = delete;
	GameEngineInput(GameEngineInput&& _other) = delete;

	GameEngineInput& operator=(const GameEngineInput& _other) = delete;
	GameEngineInput& operator=(const GameEngineInput&& _other) = delete;

	bool CreateKey(const std::string& _keyName, int _key);

	bool IsKeyDown(const std::string& _keyName);
	bool IsKeyUp(const std::string& _keyName);
	bool IsKeyPress(const std::string& _keyName);
	bool IsKeyFree(const std::string& _keyName);
	bool IsKeyAvailable(const std::string& _keyName);

	float4 GetMouseDirection();

private:
	GameEngineInput();
	void update();

private:
	static GameEngineInput* instance_;

	std::unordered_map<std::string, GameEngineKey*> keys_;

	float4 prevMousePos_;
	float4 currentMousePos_;
	float4 mouseDirection_;
};

