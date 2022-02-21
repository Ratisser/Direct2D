#pragma once

// 설명 : 
class GameEngineRenderingPipeline;
class GameEngineRenderingPipelineManager
{
private:
	static GameEngineRenderingPipelineManager* Inst;

public:
	static GameEngineRenderingPipelineManager& GetInst()
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

private:	// member Var
	std::map<std::string, GameEngineRenderingPipeline*> ResourcesMap;

public:
	// 직접 만들수 있다.
	GameEngineRenderingPipeline* Create(const std::string& _Name);
	// 파일에서 로드
	GameEngineRenderingPipeline* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineRenderingPipeline* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineRenderingPipeline* Find(const std::string& _Name);

private:
	GameEngineRenderingPipelineManager(); // default constructer 디폴트 생성자
	~GameEngineRenderingPipelineManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRenderingPipelineManager(const GameEngineRenderingPipelineManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRenderingPipelineManager(GameEngineRenderingPipelineManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineRenderingPipelineManager& operator=(const GameEngineRenderingPipelineManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRenderingPipelineManager& operator=(const GameEngineRenderingPipelineManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




