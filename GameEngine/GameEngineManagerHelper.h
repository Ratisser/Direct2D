#pragma once

#include <GameEngineBase/GameEngineSound.h>
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineIndexBufferManager.h"
#include "GameEngineRasterizerManager.h"
#include "GameEngineTextureManager.h"
#include "GameEngineRenderTargetManager.h"

#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineRasterizer.h"
#include "GameEngineRenderTarget.h"


class GameEngineManagerHelper
{
public:
	static void ManagerRelase()
	{
		GameEngineVertexBufferManager::Destroy();
		GameEngineVertexShaderManager::Destroy();
		GameEngineIndexBufferManager::Destroy();
		GameEngineRasterizerManager::Destroy();
		GameEngineRenderTargetManager::Destroy();
		GameEngineTextureManager::Destroy();
		GameEngineSound::Destroy();
	}
};
