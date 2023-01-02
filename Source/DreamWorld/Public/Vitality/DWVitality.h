// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Ability/Inventory/InventoryAgentInterface.h"

#include "DWVitality.generated.h"

class UWorldWidgetComponent;
class UVitalityInventory;
class ADWVoxelChunk;
class UVitalityInteractionComponent;
class AVoxelChunk;
class ADWVitality;
class UBoxComponent;
class UInventory;
class UAttributeSetBase;

/**
 * ������������
 */
UCLASS()
class DREAMWORLD_API ADWVitality : public AAbilityVitalityBase
{
	GENERATED_BODY()

	friend class UDWVitalityState_Death;
	friend class UDWVitalityState_Default;

public:
	// Sets default values for this actor's properties
	ADWVitality();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWorldWidgetComponent* VitalityHP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual int32 GetLimit_Implementation() const override { return 1000; }

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

	virtual void Death(IAbilityVitalityInterface* InKiller = nullptr) override;

	virtual void Revive(IAbilityVitalityInterface* InRescuer = nullptr) override;

	virtual bool CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess) override;
		
	virtual void OnCancelItem(const FAbilityItem& InItem, bool bPassive) override;

	virtual void OnAssembleItem(const FAbilityItem& InItem) override;

	virtual void OnDischargeItem(const FAbilityItem& InItem) override;

	virtual void OnDiscardItem(const FAbilityItem& InItem, bool bInPlace) override;

	virtual void OnSelectItem(const FAbilityItem& InItem) override;

	virtual void OnAuxiliaryItem(const FAbilityItem& InItem) override;

	virtual bool GenerateVoxel(const FVoxelHitResult& InVoxelHitResult) override;

	virtual bool DestroyVoxel(const FVoxelHitResult& InVoxelHitResult) override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual bool SetLevelV(int32 InLevel) override;
			
	UFUNCTION(BlueprintPure)
	UWorldWidgetComponent* GetVitalityHP() const { return VitalityHP; }

	UFUNCTION(BlueprintPure)
	UWidgetVitalityHP* GetVitalityHPWidget() const;

public:
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;
	
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
};
