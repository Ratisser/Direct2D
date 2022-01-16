#include "PreCompile.h"

#include "UserGame.h"

int __stdcall wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	GameEngineCore::Start<UserGame>();
	return 0;
}
