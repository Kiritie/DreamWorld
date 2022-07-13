// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Item/Equip/DWEquip.h"
#include "DWEquipWeapon.generated.h"

class UBoxComponent;

/**
 * ����
 */
UCLASS()
class DREAMWORLD_API ADWEquipWeapon : public ADWEquip
{
	GENERATED_BODY()

public:
	ADWEquipWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnHitTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	UFUNCTION(BlueprintCallable)
	void SetCollisionEnable(bool InValue);
	
	UFUNCTION(BlueprintPure)
	UBoxComponent* GetBoxComponent() const { return BoxComponent; }
};
