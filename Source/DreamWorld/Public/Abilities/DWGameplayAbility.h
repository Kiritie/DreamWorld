#pragma once

#include "DWTypes.h"
#include "Abilities/GameplayAbility.h"
#include "DWGameplayAbility.generated.h"

/**
 * GameplayAbility����
 */
UCLASS()
class DREAMWORLD_API UDWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDWGameplayAbility();

public:
	/** GameplayEffectr�����б� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffects")
	TMap<FGameplayTag, FDWGameplayEffectContainer> EffectContainerMap;

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;

public:
	/** ��GameplayEffect��������GameplayEffect�����淶 */
	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (AutoCreateRefTerm = "EventData"))
	virtual FDWGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FDWGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** �ӱ�ǩ��������GameplayEffect�����淶 */
	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (AutoCreateRefTerm = "EventData"))
	virtual FDWGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Ӧ��GameplayEffect�����淶 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FDWGameplayEffectContainerSpec& ContainerSpec);

	/** Ӧ��GameplayEffect���� */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData,	int32 OverrideGameplayLevel = -1);
};