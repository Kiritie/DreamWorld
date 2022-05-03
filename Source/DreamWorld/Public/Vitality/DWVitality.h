// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
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
class DREAMWORLD_API ADWVitality : public AAbilityVitalityBase, public IVoxelAgentInterface
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData) override;

	virtual FSaveData* ToData(bool bSaved = true) override;
	
	virtual void ResetData(bool bRefresh = false) override;
	
	virtual void RefreshData() override;

	virtual void Death(AActor* InKiller = nullptr) override;

	virtual void Spawn() override;

	virtual void Revive() override;

	virtual bool GenerateVoxel(const FVoxelHitResult& InVoxelHitResult, FAbilityItem& InItem) override;

	virtual bool DestroyVoxel(const FVoxelHitResult& InVoxelHitResult) override;

	virtual bool CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction) override;

public:
	UFUNCTION(BlueprintPure)
	virtual AVoxelChunk* GetOwnerChunk() const override { return OwnerChunk; }
	
	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerChunk(AVoxelChunk* InOwnerChunk) override { OwnerChunk = InOwnerChunk; }

	virtual FAbilityItem& GetGeneratingVoxelItem() override;

	virtual FVoxelItem& GetSelectedVoxelItem() override;

	UFUNCTION(BlueprintPure)
	virtual UInventory* GetInventory() const { return Inventory; }
		
	UFUNCTION(BlueprintPure)
	UWidgetVitalityHP* GetWidgetVitalityHPWidget() const;

public:
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
		
	virtual void HandleNameChanged(FName NewValue) override;

	virtual void HandleRaceIDChanged(FName NewValue) override;

	virtual void HandleLevelChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleEXPChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleHealthChanged(float NewValue, float DeltaValue = 0.f) override;
	
	virtual void HandleMaxHealthChanged(float NewValue, float DeltaValue = 0.f) override;
};
