#include "DWAssetManager.h"
#include "AbilitySystemGlobals.h"

UDWAssetManager& UDWAssetManager::Get()
{
	// ��ȡ����Ĭ�ϵ���Դ������
	UDWAssetManager* Manager = Cast<UDWAssetManager>(GEngine->AssetManager);
	if (Manager)
	{
		return *Manager;
	}
	else
	{
		UDWHelper::Debug("�뽫DefaultEngine.ini���AssetManager����ΪDWAssetManager��");
		return *NewObject<UDWAssetManager>();
	}
}

void UDWAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
