// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Pool/PoolWidgetBase.h"

#include "WidgetTaskCategory.generated.h"

class UTaskAsset;
class UTextBlock;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTaskCategory : public UPoolWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetTaskCategory(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnDespawn_Implementation(bool bRecovery) override;

	virtual void OnRefresh() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtName;

protected:
	UPROPERTY(BlueprintReadOnly)
	UTaskAsset* TaskAsset;

public:
	UTaskAsset* GetTaskAsset() const { return TaskAsset; }
};
