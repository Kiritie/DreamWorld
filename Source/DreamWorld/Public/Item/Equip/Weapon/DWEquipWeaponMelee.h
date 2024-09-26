// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWEquipWeapon.h"
#include "Ability/Interfaces/AbilityHitterInterface.h"
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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;
	
	UPROPERTY()
	TArray<AActor*> HitTargets;

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	virtual bool CanHitTarget(AActor* InTarget) const override;

	virtual void OnHitTarget(AActor* InTarget, const FHitResult& InHitResult) override;
	
	virtual void ClearHitTargets() override;

	virtual void SetHitAble(bool bValue) override;
	
public:
	UFUNCTION(BlueprintPure)
	UBoxComponent* GetBoxComponent() const { return BoxComponent; }
};
