// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/Equip/DWEquip.h"
#include "Ability/Interfaces/AbilityAttackerInterface.h"
#include "DWEquipWeapon.generated.h"

class UBoxComponent;

/**
 * ����
 */
UCLASS()
class DREAMWORLD_API ADWEquipWeapon : public ADWEquip, public IAbilityAttackerInterface
{
	GENERATED_BODY()

public:
	ADWEquipWeapon();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
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
	
	virtual void ClearHitTargets();

	virtual void SetHitAble(bool bValue);
	
public:
	UFUNCTION(BlueprintPure)
	UBoxComponent* GetBoxComponent() const { return BoxComponent; }
};
