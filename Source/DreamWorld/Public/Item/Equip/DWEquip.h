// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "DWEquip.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWEquip : public AAbilityEquipBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADWEquip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	virtual void Initialize(AAbilityCharacterBase* InOwnerCharacter) override;

	virtual void SetVisible_Implementation(bool bVisible) override;

	virtual void OnAssemble_Implementation() override;
	
	virtual void OnDischarge_Implementation() override;
};
