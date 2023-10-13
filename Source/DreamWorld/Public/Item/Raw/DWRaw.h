// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Raw/AbilityRawBase.h"
#include "DWRaw.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWRaw : public AAbilityRawBase
{
	GENERATED_BODY()
	
public:	
	ADWRaw();

public:
	virtual void Initialize_Implementation(AActor* InOwnerActor, const FAbilityItem& InItem = FAbilityItem::Empty) override;
};
