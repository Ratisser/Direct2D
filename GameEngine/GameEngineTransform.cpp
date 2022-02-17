#include "PreCompile.h"
#include "GameEngineTransform.h"

GameEngineTransform::GameEngineTransform()
	: transformData_(std::make_unique<TransformData>())
	, scale_(std::make_unique<float4>(1.0f, 1.0f, 1.0f, 1.0f))
	, rotation_(std::make_unique<float4>(float4::ZERO))
	, location_(std::make_unique<float4>(float4::ZERO))
{

}

GameEngineTransform::~GameEngineTransform()
{
}