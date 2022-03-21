#pragma once

#include <functional>
#include <map>

class GameEngineFSM
{
	class State
	{
		friend GameEngineFSM;
	public:
		State(std::function<void()> _start, std::function<void()> _update
			, std::function<void()> _init = nullptr, std::function<void()> _end = nullptr);
	private:
		std::function<void()> init_;
		std::function<void()> start_;
		std::function<void()> update_;
		std::function<void()> end_;
	};
public:
	GameEngineFSM();
	~GameEngineFSM();
	GameEngineFSM(const GameEngineFSM& _other) = delete;
	GameEngineFSM(GameEngineFSM&& _other) = delete;
	GameEngineFSM& operator=(const GameEngineFSM& _other) = delete;
	GameEngineFSM& operator=(const GameEngineFSM&& _other) = delete;

public:
	void CreateState(const std::string& _stateName
		, std::function<void()> _start, std::function<void()> _update
		, std::function<void()> _init = nullptr, std::function<void()> _end = nullptr);

	void Update();

private:
	std::map<std::string, GameEngineFSM::State*> allState_;

	State* current_;
	State* next_;
};

