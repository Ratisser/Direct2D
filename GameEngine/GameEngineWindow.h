#pragma once
#include <Windows.h>
#include <string>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>


// �з� : ����
// �뵵 : winapi�� ������ â�� �����ϴ� ��ɵ鿡 ���� 
//       ����(�ٸ� api�� ����� �� �Ը��� �´� Ŭ������ �ѹ� ���մ�.)���ϰ�
//       ���� �Լ����� �����Ѵ�.
// ���� : 
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

	static float GetFPS() { return fps_; }

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
	static float fps_;

	std::string className_;
	std::string windowTitle_;
	HINSTANCE hInstance_;
	HWND windowhandle_;
	HDC devicecontext_;

	float4 size_;
	float4 pos_;
};
