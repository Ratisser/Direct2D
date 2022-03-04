#pragma once

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineObjectBase
{
private:	// member Var
	// true라고 하는순간 메모리적으로 파괴될겁니다.
	bool bRelease_;
	bool bUpdate_;
	bool bDebuging_;
	GameEngineObjectBase* parent_;
	int order_;

public:
	bool IsNull() 
	{
		return nullptr == this;
	}

	void SetParent(GameEngineObjectBase* _parent)
	{
		parent_ = _parent;
	}

public:
	bool IsDeath()
	{
		if (nullptr == parent_)
		{
			return bRelease_;
		}

		return true == bRelease_ || parent_->bRelease_;
	}

	bool IsUpdate()
	{
		if (nullptr == parent_)
		{
			return false == IsDeath() && bUpdate_;
		}

		return false == IsDeath() && true == bUpdate_ && parent_->bUpdate_;
	}


	void Death()
	{
		bRelease_ = true;
	}

	void On()
	{
		bUpdate_ = true;
	}

	void Off()
	{
		bUpdate_ = false;
	}

	void DebugOn()
	{
		bDebuging_ = true;
	}

	void DebugOff()
	{
		bDebuging_ = false;
	}

	void IsDebugCheck()
	{
		if (true == bDebuging_)
		{
			int a = 0;
		}
	}

	void SetOrder(int _Order)
	{
		order_ = _Order;
	}

	int GetOrder()
	{
		return order_;
	}

public:
	GameEngineObjectBase(); // default constructer 디폴트 생성자
	virtual ~GameEngineObjectBase(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineObjectBase(const GameEngineObjectBase& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineObjectBase(GameEngineObjectBase&& _other) = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineObjectBase& operator=(const GameEngineObjectBase& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineObjectBase& operator=(const GameEngineObjectBase&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
};

