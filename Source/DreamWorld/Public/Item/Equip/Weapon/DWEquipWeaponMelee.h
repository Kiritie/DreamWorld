// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWEquipWeapon.h"
#include "Ability/Hitter/AbilityHitterInterface.h"
#include "DWEquipWeaponMelee.generated.h"

class UBoxComponent;

/**
 * ����
 */
UCLASS()
class DREAMWORLD_API ADWEquipWeaponMelee : public ADWEquipWeapon, public IAbilityHitterInterface
{
	GENERATED_BODY()

public:
	ADWEquipWeaponMelee();

protected:
	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnDespawn_Implementation(bool bRecovery) override;

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	virtual bool CanHitTarget(AActor* InTarget) const override;

	virtual void OnHitTarget(AActor* InTarget, const FHitResult& InHitResult) override;

	virtual bool IsHitAble() const override;
	
	virtual void SetHitAble(bool bValue) override;

	virtual void ClearHitTargets() override;

	virtual TArray<AActor*> GetHitTargets() const override;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* AttackHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* AttackHitEffect;
	
	UPROPERTY()
	TArray<AActor*> HitTargets;

public:
	UFUNCTION(BlueprintPure)
	UBoxComponent* GetBoxComponent() const { return BoxComponent; }
};
