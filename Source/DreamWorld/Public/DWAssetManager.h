#pragma once

#include "DreamWorld.h"
#include "Engine/AssetManager.h"
#include "DWAssetManager.generated.h"

/**
 * ��Դ������
 */
UCLASS()
class DREAMWORLD_API UDWAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UDWAssetManager() {}

public:
	/** ��ȡ��ǰ��Դ������ʵ�� */
	static UDWAssetManager& Get();

public:
	virtual void StartInitialLoading() override;
};