// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Inventory/InventoryAgentInterface.h"
#include "Voxel/Agent/VoxelAgentInterface.h"

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
class DREAMWORLD_API ADWVitality : public AAbilityVitalityBase, public IVoxelAgentInterface, public IInventoryAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADWVitality();

protected:
	// stats
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	AVoxelChunk* OwnerChunk;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetVitalityHPComponent* WidgetVitalityHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventory* Inventory;

protected:
	FVoxelItem SelectedVoxelItem;

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
	
	virtual void ResetData() override;

	virtual void Death(AActor* InKiller = nullptr) override;

	virtual void Revive() override;

	virtual bool GenerateVoxel(const FVoxelHitResult& InVoxelHitResult, FAbilityItem& InItem) override;

	virtual bool DestroyVoxel(const FVoxelHitResult& InVoxelHitResult) override;

	virtual bool CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

public:
	virtual void SetNameV(FName InName) override;

	virtual void SetRaceID(FName InRaceID) override;

	virtual void SetLevelV(int32 InLevel) override;

	virtual void SetEXP(int32 InEXP) override;

	UFUNCTION(BlueprintPure)
	virtual AVoxelChunk* GetOwnerChunk() const override { return OwnerChunk; }
	
	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerChunk(AVoxelChunk* InOwnerChunk) override { OwnerChunk = InOwnerChunk; }

	virtual FAbilityItem& GetGeneratingVoxelItem() override;

	virtual FVoxelItem& GetSelectedVoxelItem() override;

	UFUNCTION(BlueprintPure)
	virtual UInventory* GetInventory() const override { return Inventory; }
		
	UFUNCTION(BlueprintPure)
	UWidgetVitalityHP* GetWidgetVitalityHPWidget() const;

public:
	virtual void OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData) override;
	
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
};
