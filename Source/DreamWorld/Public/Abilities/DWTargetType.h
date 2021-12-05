#pragma once

#include "Abilities/GameplayAbilityTypes.h"
#include "DreamWorld/DreamWorld.h"
#include "DWTargetType.generated.h"
 
 class IVitality;

/**
 * ����Ŀ�����
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class DREAMWORLD_API UDWTargetType : public UObject
{
	GENERATED_BODY()

public:
	UDWTargetType() {}

	/** ��ȡĿ�� */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AActor* OwningActor, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/**
 * ����Ŀ��_ʹ��ӵ����
 */
UCLASS(NotBlueprintable)
class DREAMWORLD_API UDWTargetType_UseOwner : public UDWTargetType
{
	GENERATED_BODY()

public:
	UDWTargetType_UseOwner() {}
	
	virtual void GetTargets_Implementation(AActor* OwningActor, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/**
 * ����Ŀ��_ʹ���¼�����
 */
UCLASS(NotBlueprintable)
class DREAMWORLD_API UDWTargetType_UseEventData : public UDWTargetType
{
	GENERATED_BODY()

public:
	UDWTargetType_UseEventData() {}

	virtual void GetTargets_Implementation(AActor* OwningActor, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
