#pragma once
#include "GameEngineRenderer.h"
#include <GameEngine\eImagePivot.h>


// ???? :
class GameEngineTexture;
class GameEngineFolderTexture;
class GameEngineImageRenderer : public GameEngineRenderer
{
	friend class Animation2D;
private:
	struct Animation2D
	{
	public:
		GameEngineFolderTexture* FolderTextures_;
		std::string Name_;
		float InterTime_;
		float CurTime_;

		bool IsEnd_;
		bool Loop_;
		bool bImageScale_;
		int CurFrame_;
		int StartFrame_;
		int EndFrame_;

		GameEngineImageRenderer* Renderer;

		std::map<int, std::vector<std::function<void()>>> FrameCallBack_;
		std::vector<std::function<void()>> EndCallBack_;
		std::vector<std::function<void()>> StartCallBack_;

	public:
		void Reset();
		void CallStart();
		void CallEnd();
		void CallFrame();
		void Update(float _DeltaTime);

		void FrameUpdate();
		void ReverseFrameUpdate();
	};

public:
	// constrcuter destructer
	GameEngineImageRenderer();
	~GameEngineImageRenderer();

	// delete Function
	GameEngineImageRenderer(const GameEngineImageRenderer& _Other) = delete;
	GameEngineImageRenderer(GameEngineImageRenderer&& _Other) noexcept = delete;
	GameEngineImageRenderer& operator=(const GameEngineImageRenderer& _Other) = delete;
	GameEngineImageRenderer& operator=(GameEngineImageRenderer&& _Other) noexcept = delete;

public:
	void SetImage(const std::string& _ImageName);
	void SetIndex(const int Index);

	void CreateAnimation(const std::string& _Name, int _StartFrame, int _EndFrame, float _InterTime, bool _Loop = true);
	void CreateAnimationFolder(const std::string& _Name, const std::string& _FolderTexName, float _InterTime = 0.034f, bool _Loop = true, bool _bImageScale = true);
	void CreateAnimationFolder(const std::string& _FolderTexName, float _InterTime = 0.034f, bool _Loop = true, bool _bImageScale = true);
	void CreateAnimationFolderReverse(const std::string& _Name, const std::string& _FolderTexName, float _InterTime = 0.034f, bool _Loop = true, bool _bImageScale = true);
	void CreateAnimationFolderReverse(const std::string& _FolderTexName, float _InterTime = 0.034f, bool _Loop = true, bool _bImageScale = true);
	void ChangeAnimation(const std::string& _Name, bool _IsForce = false);
	Animation2D* GetCurrentAnimation() const { return CurAnimation_; }

	void SetStartCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetEndCallBack(const std::string& _Name, std::function<void()> _CallBack);
	void SetFrameCallBack(const std::string& _Name, int _Index, std::function<void()> _CallBack);

	void SetFlip(bool _bHorizon, bool _bVertical);

	void SetPivot(eImagePivot _pivot) { pivot_ = _pivot; }

	void SetColor(float4 _color) { color_ = _color; }
	float4 GetColor() const { return color_; }

	void SetAddColor(float4 _color) { addColor_ = _color; }
	float4 GetAddColor() const { return addColor_; }

	void SetAddUV(float4 _uv) { addUV_ = _uv; }
	float4 GetAddUV() const { return addUV_; }
	void AddAddUV(float4 _uv) { addUV_ += _uv; }

	void Stop() { bStop_ = true; }
	void Resume() { bStop_ = false; }

protected:
	void Update(float _DeltaTime) override;
	void Start() override;

private:
	std::map<std::string, Animation2D*> AllAnimations_;
	Animation2D* CurAnimation_;

	float4 CutData_;
	float4 color_;
	float4 addColor_;
	float4 addUV_;

	bool bFlipHorizontal_;
	bool bFlipVertical_;
	bool bStop_;

	eImagePivot pivot_;
};

