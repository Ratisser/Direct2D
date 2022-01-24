#pragma once

#include <GameEngineBase/GameEngineSound.h>
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineIndexBufferManager.h"
//#include "GameEngineReasterizerManager.h"
#include "GameEngineTextureManager.h"

#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineTexture.h"
//#include "GameEngineReasterizer.h"


class GameEngineManagerHelper
{
public:
	static void ManagerRelase()
	{
		GameEngineIndexBufferManager::Destroy();
		GameEngineVertexShaderManager::Destroy();
		GameEngineVertexBufferManager::Destroy();
		//GameEngineReasterizerManager::Destroy();
		GameEngineTextureManager::Destroy();
		GameEngineSound::Destroy();
	}
};
