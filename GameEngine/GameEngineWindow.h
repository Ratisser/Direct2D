#pragma once
#include <Windows.h>
#include <string>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>


// 분류 : 엔진
// 용도 : winapi의 윈도우 창을 제어하는 기능들에 대한 
//       랩핑(다른 api의 기능을 내 입맛에 맞는 클래스로 한번 감쌌다.)을하고
//       편의 함수들을 제공한다.
// 설명 : 
class GameEngineWindow : public GameEngineObjectNameBase
{
private:
	static GameEngineWindow* Inst;

public:
	static GameEngineWindow& GetInst() 
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

public:
	friend LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	static inline void SetMessageCallBack(std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> _CallBack)
	{
		MessageCallBack_ = _CallBack;
	}

public:
	void CreateMainWindow(const std::string& _titlename, const float4& _size, const float4& _pos);
	void SetSizeAndPos(const float4& _size, const float4& _pos);
	void Loop(void(*_loopFunc)());


public:
	HWND  GetWindowHWND()
	{
		return windowhandle_;
	}

	HDC  GetWindowDC()
	{
		return devicecontext_;
	}

	float4 GetSize() {
		return size_;
	}

	float4 GetPos() {
		return pos_;
	}


private:
	int CreateMainWindowClass();

private:
	GameEngineWindow();
	~GameEngineWindow();

private:
	static std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> MessageCallBack_;

	std::string className_;
	std::string windowTitle_;
	HINSTANCE hInstance_;
	HWND windowhandle_;
	HDC devicecontext_;

	float4 size_;
	float4 pos_;
};
