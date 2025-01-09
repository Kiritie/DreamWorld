// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ability/Vitality/AbilityVitalityBase.h"

#include "DWVitality.generated.h"

class UEventHandle_VoxelWorldModeChanged;
class UWorldWidgetComponent;
class UAbilityVitalityInventoryBase;
class ADWVoxelChunk;
class UInteractionComponent;
class AVoxelChunk;
class ADWVitality;
class UBoxComponent;
class UAbilityInventoryBase;
class UAttributeSetBase;

/**
 * ������������
 */
UCLASS()
class DREAMWORLD_API ADWVitality : public AAbilityVitalityBase
{
	GENERATED_BODY()

public:
	ADWVitality(const FObjectInitializer& ObjectInitializer);

public:
	virtual int32 GetLimit_Implementation() const override { return -1; }

	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void Death(IAbilityVitalityInterface* InKiller = nullptr) override;

	virtual void Revive(IAbilityVitalityInterface* InRescuer = nullptr) override;

	virtual bool CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent) override;

	virtual void OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive) override;

	virtual void OnAdditionItem(const FAbilityItem& InItem) override;

	virtual void OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess) override;
		
	virtual void OnDeactiveItem(const FAbilityItem& InItem, bool bPassive) override;

	virtual void OnDiscardItem(const FAbilityItem& InItem, bool bInPlace) override;

	virtual void OnSelectItem(const FAbilityItem& InItem) override;

	virtual void OnAuxiliaryItem(const FAbilityItem& InItem) override;

	virtual bool OnGenerateVoxel(EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult) override;

	virtual bool OnDestroyVoxel(EInputInteractEvent InInteractEvent, const FVoxelHitResult& InHitResult) override;

protected:
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;

	UFUNCTION()
	virtual void OnWorldModeChanged(UObject* InSender, UEventHandle_VoxelWorldModeChanged* InEventHandle);

public:
	virtual void HandleDamage(const FGameplayAttribute& DamageAttribute, float DamageValue, float DefendValue, bool bHasCrited, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;

	virtual void HandleRecovery(const FGameplayAttribute& RecoveryAttribute, float RecoveryValue, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;

	virtual void HandleInterrupt(const FGameplayAttribute& InterruptAttribute, float InterruptDuration, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UWorldWidgetComponent* VitalityHP;

public:
	virtual void SetNameA(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual bool SetLevelA(int32 InLevel) override;
			
	UFUNCTION(BlueprintPure)
	UWorldWidgetComponent* GetVitalityHP() const { return VitalityHP; }

	UFUNCTION(BlueprintPure)
	UWidgetVitalityHP* GetVitalityHPWidget() const;
};
