#pragma once
#include "GameEngineDebug.h"
#include "GameEngineObjectNameBase.h"
#include "GameEngineTime.h"
#include <map>

class StateInfo
{
public:
	std::string _NextState;
	float Time;

public:
	StateInfo()
		: _NextState("")
		, Time(0.0f)
	{

	}

	StateInfo(const char* _Name)
		: _NextState(_Name)
		, Time(0.0f)
	{

	}

	StateInfo(const char* _Name, float _NextTime)
		: _NextState(_Name)
		, Time(_NextTime)
	{

	}

	StateInfo(const std::string& _Name)
		: _NextState(_Name)
		, Time(0.0f)
	{

	}

	StateInfo(const std::string& _Name, float _NextTime)
		: _NextState(_Name)
		, Time(_NextTime)
	{

	}

};



// 전환된다 진행된다
template<typename FSMType>
class GameEngineFSM
{
	class State : public GameEngineObjectNameBase
	{
		// 이렇게 전방선언 할수도 있다.
		friend class GameEngineFSM;

	public:
		GameEngineFSM* parent_;

		float StateTime; // 이 스테이트에 진입한지 몇초가 됐다.
		StateInfo(FSMType::* StateStart)(StateInfo _Ptr);
		StateInfo(FSMType::* StateUpdate)(StateInfo _Ptr);

	public:

		void CallStart()
		{
			if (nullptr == parent_->ObjectPtr_)
			{
				GameEngineDebug::AssertFalse();
				return;
			}

			(parent_->ObjectPtr_->*StateStart)({ GetName(), 0.0f });
			StateTime = 0.0f;
		}
		void CallUpdate()
		{
			if (nullptr == parent_->ObjectPtr_)
			{
				GameEngineDebug::AssertFalse();
				return;
			}

			StateTime += GameEngineTime::GetInst().GetDeltaTime();
			StateInfo Return = (parent_->ObjectPtr_->*StateUpdate)({GetName(), StateTime });

			if (Return._NextState.size() == 0)
			{
				return;
			}

			if (Return._NextState == "")
			{
				return;
			}

			if (nullptr == parent_->FindState(Return._NextState))
			{
				GameEngineDebug::AssertFalse();
			}

			// 넘어갈수 있는곳인데.
			if (0 >= Return.Time)
			{
				parent_->ChangeState(Return._NextState);
			}

			// 넘어가는데 몇초를 기다려야 된다는것을 알게 되었다.
			parent_->nextState_ = Return;
		}


	public:
		void Update() {}

	public:
		State(GameEngineFSM* _parent)
			: parent_(_parent)
			, StateStart(nullptr)
			, StateUpdate(nullptr)
			, StateTime(0.0f)
		{

		}
	};

private:
	std::map<std::string, State*> allState_;
	FSMType* ObjectPtr_;
	State* curState_;
	StateInfo nextState_;

private:	// member Var

public:
	friend State;

public:
	GameEngineFSM(FSMType* _StateObject)
		: curState_(nullptr)
		, ObjectPtr_(_StateObject)
	{
	}// default constructer 디폴트 생성자

	~GameEngineFSM()
	{
		typename std::map<std::string, State*>::iterator Start = allState_.begin();
		typename std::map<std::string, State*>::iterator End = allState_.end();

		for (; Start != End; ++Start)
		{
			delete Start->second;
		}
		allState_.clear();
	}// default destructer 디폴트 소멸자

public:		// delete constructer
	GameEngineFSM(const GameEngineFSM& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineFSM(GameEngineFSM&& _other) noexcept
	{
	}// default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	GameEngineFSM& operator=(const GameEngineFSM& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineFSM& operator=(const GameEngineFSM&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자



public:
	void CreateState(const std::string& _name, StateInfo(FSMType::* _StartFunc)(StateInfo), StateInfo(FSMType::* _UpdateFunc)(StateInfo))
	{
		if (nullptr != FindState(_name))
		{
			GameEngineDebug::AssertFalse();
		}

		State* NewState = new State(this);
		NewState->SetName(_name);

		NewState->StateUpdate = _UpdateFunc;
		NewState->StateStart = _StartFunc;
		allState_.insert(std::map<std::string, State*>::value_type(_name, NewState));
		return;
	}

	void ChangeState(const std::string& _Name)
	{
		curState_ = FindState(_Name);

		if (nullptr == curState_)
		{
			GameEngineDebug::AssertFalse();
			return;
		}
		curState_->CallStart();
	}


private:
	State* FindState(const std::string& _name)
	{
		typename std::map<std::string, State*>::iterator FindAniIter = allState_.find(_name);

		if (FindAniIter == allState_.end())
		{
			return nullptr;
		}

		return FindAniIter->second;
	}

	bool IsCurStateName(const std::string& _Name)
	{
		if (nullptr == curState_)
		{
			GameEngineDebug::AssertFalse();
			return false;
		}

		if (curState_->GetName() == _Name)
		{
			return true;
		}

		return false;
	}

public:
	void Update()
	{
		if (nullptr == curState_)
		{
			GameEngineDebug::AssertFalse();
			return;
		}

		//if (0 < nextState_.Time)
		//{
		//	nextState_.Time -= GameEngineTime::GetInst().GetDeltaTime();
		//	return;
		//}
		//else
		//{
		//	if ("" != nextState_._NextState)
		//	{
		//		ChangeState(nextState_._NextState);
		//		nextState_._NextState = "";
		//	}
		//}

		curState_->CallUpdate();
	}
};