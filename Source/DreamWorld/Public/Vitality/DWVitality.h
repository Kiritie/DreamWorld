// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Inventory/InventoryAgentInterface.h"

#include "DWVitality.generated.h"

class ADWVoxelChunk;
class UVitalityInteractionComponent;
class AVoxelChunk;
class ADWCharacter;
class UBoxComponent;
class UWidgetVitalityHPComponent;
class UInventory;
class UAttributeSetBase;

/**
 * ������������
 */
UCLASS()
class DREAMWORLD_API ADWVitality : public AAbilityVitalityBase, public IInventoryAgentInterface
{
	GENERATED_BODY()

	friend class UDWVitalityState_Death;
	friend class UDWVitalityState_Default;

public:
	// Sets default values for this actor's properties
	ADWVitality();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetVitalityHPComponent* WidgetVitalityHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventory* Inventory;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData) override;

	virtual FSaveData* ToData() override;

	virtual void Death(IAbilityVitalityInterface* InKiller = nullptr) override;

	virtual void Revive(IAbilityVitalityInterface* InRescuer = nullptr) override;

	virtual bool CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual bool GenerateVoxel(FVoxelItem& InVoxelItem) override;

	virtual bool GenerateVoxel(FVoxelItem& InVoxelItem, const FVoxelHitResult& InVoxelHitResult) override;

	virtual bool DestroyVoxel(FVoxelItem& InVoxelItem) override;

	virtual bool DestroyVoxel(const FVoxelHitResult& InVoxelHitResult) override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual void SetLevelV(int32 InLevel) override;

	virtual void SetEXP(int32 InEXP) override;

	UFUNCTION(BlueprintPure)
	virtual UInventory* GetInventory() const override { return Inventory; }
		
	UFUNCTION(BlueprintPure)
	UWidgetVitalityHP* GetWidgetVitalityHPWidget() const;

public:
	virtual void OnInventorySlotSelected(UInventorySlot* InInventorySlot);
	
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;
	
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, IAbilityVitalityInterface* SourceVitality) override;
};
