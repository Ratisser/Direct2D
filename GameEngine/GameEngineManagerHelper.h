#pragma once

#include <GameEngineBase/GameEngineSoundManager.h>
#include "GameEngineConstantBufferManager.h"
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineIndexBufferManager.h"
#include "GameEngineRasterizerManager.h"
#include "GameEnginePixelShaderManager.h"
#include "GameEngineRenderTargetManager.h"
#include "GameEngineTextureManager.h"
#include "GameEngineSamplerManager.h"
#include "GameEngineBlenderManager.h"

#include "GameEngineRenderingPipelineManager.h"

#include "GameEngineConstantBuffer.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineRasterizer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineBlender.h"

#include "GameEngineRenderingPipeline.h"


class GameEngineManagerHelper 
{
public:
	static void ManagerRelease()
	{
		GameEngineSoundManager::Destroy();

		GameEngineRenderingPipelineManager::Destroy();
		GameEngineConstantBufferManager::Destroy();
		GameEngineIndexBufferManager::Destroy();
		GameEngineVertexBufferManager::Destroy();
		GameEngineVertexShaderManager::Destroy();
		GameEngineRasterizerManager::Destroy();
		GameEngineRenderTargetManager::Destroy();
		GameEnginePixelShaderManager::Destroy();
		GameEngineTextureManager::Destroy();
		GameEngineSamplerManager::Destroy();
		GameEngineBlenderManager::Destroy();
	}
};
