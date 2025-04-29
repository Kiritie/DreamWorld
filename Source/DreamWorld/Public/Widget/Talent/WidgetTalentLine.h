// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Pool/PoolWidgetBase.h"

#include "WidgetTalentLine.generated.h"

class USizeBox;
class UVerticalBox;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTalentLine : public UPoolWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetTalentLine(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnRefresh() override;
};
