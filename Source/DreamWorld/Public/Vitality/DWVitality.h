// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/Components/InteractionComponent.h"
#include "Voxel/Agent/VoxelAgentInterface.h"

#include "DWVitality.generated.h"

class ADWVoxelChunk;
class UAbilityVitalityInteractionComponent;
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
class DREAMWORLD_API ADWVitality : public AAbilityVitalityBase, public IVoxelAgentInterface, public IInteractionInterface, public IAbilitySystemInterface
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
	UAbilityVitalityInteractionComponent* Interaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetVitalityHPComponent* WidgetVitalityHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UInventory* Inventory;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Serialize(FArchive& Ar) override;

	virtual void LoadData(FSaveData* InSaveData) override;

	virtual FSaveData* ToData(bool bSaved = true) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetData(bool bRefresh = false) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void RefreshData() override;

	UFUNCTION(BlueprintCallable)
	virtual void Death(AActor* InKiller = nullptr) override;

	UFUNCTION(BlueprintCallable)
	virtual void Spawn() override;

	UFUNCTION(BlueprintCallable)
	virtual void Revive() override;

	virtual bool CanInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction) override;

public:
	UFUNCTION(BlueprintPure)
	virtual AVoxelChunk* GetOwnerChunk() const override { return OwnerChunk; }
	
	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerChunk(AVoxelChunk* InOwnerChunk) override { OwnerChunk = InOwnerChunk; }

	UFUNCTION(BlueprintPure)
	virtual UInventory* GetInventory() const { return Inventory; }
		
	UFUNCTION(BlueprintPure)
	UWidgetVitalityHP* GetWidgetVitalityHPWidget() const;

	UFUNCTION(BlueprintPure)
	virtual UInteractionComponent* GetInteractionComponent() const override;

public:
	virtual void HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
		
	virtual void HandleNameChanged(const FString& NewValue) override;

	virtual void HandleRaceIDChanged(const FString& NewValue) override;

	virtual void HandleLevelChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleEXPChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleHealthChanged(float NewValue, float DeltaValue = 0.f) override;
	
	virtual void HandleMaxHealthChanged(float NewValue, float DeltaValue = 0.f) override;
};
