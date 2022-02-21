#pragma once

// ���� : 
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
	// ���� ����� �ִ�.
	GameEngineRenderingPipeline* Create(const std::string& _Name);
	// ���Ͽ��� �ε�
	GameEngineRenderingPipeline* Load(const std::string& _Path);
	// �̸� ���� ����
	GameEngineRenderingPipeline* Load(const std::string& _Name, const std::string& _Path);
	// ��Ͽ��� ã�´�.
	GameEngineRenderingPipeline* Find(const std::string& _Name);

private:
	GameEngineRenderingPipelineManager(); // default constructer ����Ʈ ������
	~GameEngineRenderingPipelineManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineRenderingPipelineManager(const GameEngineRenderingPipelineManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineRenderingPipelineManager(GameEngineRenderingPipelineManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineRenderingPipelineManager& operator=(const GameEngineRenderingPipelineManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineRenderingPipelineManager& operator=(const GameEngineRenderingPipelineManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




