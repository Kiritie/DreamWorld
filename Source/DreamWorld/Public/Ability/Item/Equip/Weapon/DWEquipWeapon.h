// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "DreamWorld/DreamWorld.h"
#include "DWEquipWeapon.generated.h"

class UBoxComponent;

/**
 * ����
 */
UCLASS()
class DREAMWORLD_API ADWEquipWeapon : public AAbilityEquipBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADWEquipWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
			
	UFUNCTION(BlueprintImplementableEvent)
	void OnHitTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCollisionEnable(bool InValue);
	
	UFUNCTION(BlueprintPure)
	UBoxComponent* GetBoxComponent() const { return BoxComponent; }
};
