#pragma once

#include <functional>
#include <map>

class GameEngineFSM
{
	class State
	{
		friend GameEngineFSM;
	public:
		State(const std::string& _stateName, std::function<void(float)> _start, std::function<void(float)> _update, std::function<void(float)> _end = nullptr);
	private:
		std::string stateName_;
		std::function<void(float)> start_;
		std::function<void(float)> update_;
		std::function<void(float)> end_;
	};

public:
	GameEngineFSM();
	~GameEngineFSM();
	GameEngineFSM(const GameEngineFSM& _other) = delete;
	GameEngineFSM(GameEngineFSM&& _other) = delete;
	GameEngineFSM& operator=(const GameEngineFSM& _other) = delete;
	GameEngineFSM& operator=(const GameEngineFSM&& _other) = delete;

public:
	inline void operator<<(const std::string& _rhs) { ChangeState(_rhs); }

public:
	void CreateState(const std::string& _stateName, std::function<void(float)> _start, std::function<void(float)> _update, std::function<void(float)> _end = nullptr);

	void ChangeState(const std::string& _stateName);

	void Update(float _deltaTime);

	inline std::string& GetCurrentStateName() { return currentState_->stateName_; }

private:
	std::map<std::string, GameEngineFSM::State*> allState_;

	State* currentState_;
	State* nextState_;
};

