#pragma once
class GameEngineString
{
private:
	GameEngineString(); // default constructer 디폴트 생성자
	virtual ~GameEngineString(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineString(const GameEngineString& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineString(GameEngineString&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineString& operator=(const GameEngineString& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineString& operator=(const GameEngineString&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	static void toupper(std::string& Text);
};

