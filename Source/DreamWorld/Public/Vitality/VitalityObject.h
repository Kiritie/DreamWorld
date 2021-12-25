// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "Vitality/Vitality.h"
#include "AbilitySystemInterface.h"
#include "Abilities/DWAbilitySystemComponent.h"
#include "Interaction/Interaction.h"
#include "Interaction/Components/InteractionComponent.h"

#include "VitalityObject.generated.h"

class UVitalityInteractionComponent;
class AChunk;
class ADWCharacter;
class UBoxComponent;
class UWidgetVitalityHPComponent;
class UInventory;
class UDWAttributeSet;

/**
 * ������������
 */
UCLASS()
class DREAMWORLD_API AVitalityObject : public AActor, public IVitality, public IInteraction, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVitalityObject();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStates")
	bool bDead;

protected:
	// stats
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	FName ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	FString RaceID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	int32 Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	int32 EXP;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	int32 BaseEXP;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	int32 EXPFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VitalityStats")
	AChunk* OwnerChunk;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UVitalityInteractionComponent* Interaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetVitalityHPComponent* WidgetVitalityHP;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UDWAbilitySystemComponent* AbilitySystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UDWAttributeSet* AttributeSet;

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
	
	UFUNCTION(BlueprintCallable)
	virtual void AddWorldText(FString InContent, EWorldTextType InContentType, EWorldTextStyle InContentStyle) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual FGameplayAbilitySpecHandle AcquireAbility(TSubclassOf<UDWGameplayAbility> InAbility, int32 InLevel = 1) override;

	UFUNCTION(BlueprintCallable)
	virtual bool ActiveAbility(FGameplayAbilitySpecHandle AbilityHandle, bool bAllowRemoteActivation = false) override;

	UFUNCTION(BlueprintCallable)
	virtual bool ActiveAbilityByClass(TSubclassOf<UDWGameplayAbility> AbilityClass, bool bAllowRemoteActivation = false) override;

	UFUNCTION(BlueprintCallable)
	virtual bool ActiveAbilityByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation = false) override;

	UFUNCTION(BlueprintCallable)
	virtual void CancelAbility(UDWGameplayAbility* Ability) override;

	UFUNCTION(BlueprintCallable)
	virtual void CancelAbilityByHandle(const FGameplayAbilitySpecHandle& AbilityHandle) override;

	UFUNCTION(BlueprintCallable)
	virtual void CancelAbilities(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags, UDWGameplayAbility* Ignore=nullptr) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void CancelAllAbilities(UDWGameplayAbility* Ignore=nullptr) override;
	
	UFUNCTION(BlueprintCallable)
	virtual FActiveGameplayEffectHandle ApplyEffectByClass(TSubclassOf<UGameplayEffect> EffectClass) override;
	
	UFUNCTION(BlueprintCallable)
	virtual FActiveGameplayEffectHandle ApplyEffectBySpecHandle(const FGameplayEffectSpecHandle& SpecHandle) override;
		
	UFUNCTION(BlueprintCallable)
	virtual FActiveGameplayEffectHandle ApplyEffectBySpec(const FGameplayEffectSpec& Spec) override;

	UFUNCTION(BlueprintCallable)
	virtual bool RemoveEffect(FActiveGameplayEffectHandle Handle, int32 StacksToRemove=-1) override;

	UFUNCTION(BlueprintPure)
	virtual void GetActiveAbilities(FGameplayTagContainer AbilityTags, TArray<UDWGameplayAbility*>& ActiveAbilities) override;

	UFUNCTION(BlueprintPure)
	virtual bool GetAbilityInfo(TSubclassOf<UDWGameplayAbility> AbilityClass, FDWAbilityInfo& OutAbilityInfo) override;

	UFUNCTION(BlueprintCallable)
	virtual void ModifyHealth(float InDeltaValue) override;
				
	UFUNCTION(BlueprintCallable)
	virtual void ModifyEXP(float InDeltaValue) override;

	virtual bool CanInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction) override;

	virtual void OnInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction) override;

public:
	UFUNCTION(BlueprintPure)
	virtual bool IsDead() const override;

	UFUNCTION(BlueprintPure)
	virtual FString GetNameC() const override { return Name; }

	UFUNCTION(BlueprintCallable)
	virtual void SetNameC(const FString& InName) override;
	
	UFUNCTION(BlueprintPure)
	virtual FString GetRaceID() const override { return RaceID; }

	UFUNCTION(BlueprintCallable)
	virtual void SetRaceID(const FString& InRaceID) override;

	UFUNCTION(BlueprintCallable)
	virtual void SetLevelC(int32 InLevel) override;

	UFUNCTION(BlueprintPure)
	virtual int32 GetLevelC() const override { return Level; }

	UFUNCTION(BlueprintPure)
	virtual int32 GetEXP() const override { return EXP; }
		
	UFUNCTION(BlueprintCallable)
	virtual void SetEXP(int32 InEXP) override;
		
	UFUNCTION(BlueprintPure)
	virtual int32 GetBaseEXP() const override { return BaseEXP; }
			
	UFUNCTION(BlueprintPure)
	virtual int32 GetEXPFactor() const override { return EXPFactor; }

	UFUNCTION(BlueprintPure)
	virtual int32 GetMaxEXP() const override;

	UFUNCTION(BlueprintPure)
	virtual int32 GetTotalEXP() const override;

	UFUNCTION(BlueprintPure)
	virtual FString GetHeadInfo() const override;
			
	UFUNCTION(BlueprintPure)
	virtual float GetHealth() const override;
					
	UFUNCTION(BlueprintCallable)
	virtual void SetHealth(float InValue) override;

	UFUNCTION(BlueprintPure)
	virtual float GetMaxHealth() const override;
		
	UFUNCTION(BlueprintCallable)
	virtual void SetMaxHealth(float InValue) override;
	
	UFUNCTION(BlueprintPure)
	virtual float GetPhysicsDamage() const override;
	
	UFUNCTION(BlueprintPure)
	virtual float GetMagicDamage() const override;
						
	UFUNCTION(BlueprintPure)
	FVitalityData GetVitalityData() const;

	UFUNCTION(BlueprintPure)
	AChunk* GetOwnerChunk() const { return OwnerChunk; }
	
	UFUNCTION(BlueprintCallable)
	void SetOwnerChunk(AChunk* InOwnerChunk) { OwnerChunk = InOwnerChunk; }

	UFUNCTION(BlueprintPure)
	virtual UInventory* GetInventory() const override { return Inventory; }
		
	UFUNCTION(BlueprintPure)
	UWidgetVitalityHP* GetWidgetVitalityHPWidget() const;

	UFUNCTION(BlueprintPure)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystem;}

	UFUNCTION(BlueprintPure)
	virtual UInteractionComponent* GetInteractionComponent() const override;

public:
	virtual void HandleDamage(EDWDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor) override;
		
	virtual void HandleNameChanged(const FString& NewValue) override;

	virtual void HandleRaceIDChanged(const FString& NewValue) override;

	virtual void HandleLevelChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleEXPChanged(int32 NewValue, int32 DeltaValue = 0) override;

	virtual void HandleHealthChanged(float NewValue, float DeltaValue = 0.f) override;
	
	virtual void HandleMaxHealthChanged(float NewValue, float DeltaValue = 0.f) override;
};
