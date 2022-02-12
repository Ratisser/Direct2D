#include "PreCompile.h"
#include "GameEngineString.h"
#include "GameEngineDebug.h"

GameEngineString::GameEngineString() // default constructer 디폴트 생성자
{

}

GameEngineString::~GameEngineString() // default destructer 디폴트 소멸자
{

}

GameEngineString::GameEngineString(GameEngineString&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}



void GameEngineString::toupper(std::string& Text)
{
	std::transform(Text.begin(), Text.end(), Text.begin(), ::toupper);
}

void GameEngineString::StringToWString(const std::string& _Text, std::wstring& _Out)
{
	int Size = MultiByteToWideChar(CP_ACP, 0, _Text.c_str(), _Text.size(), nullptr, 0);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return;
	}

	_Out.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, _Text.c_str(), _Text.size(), &_Out[0], Size);

	if (Size == 0)
	{
		GameEngineDebug::MsgBoxError("스트링 변환에 실패했습니다.");
		return;
	}
}