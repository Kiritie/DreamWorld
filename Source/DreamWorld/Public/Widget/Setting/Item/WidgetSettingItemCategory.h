// Fill out your copyright notice in the Description Item of Project Settings.

#pragma once
#include "Blueprint/UserWidget.h"
#include "ObjectPool/ObjectPoolInterface.h"

#include "WidgetSettingItemCategory.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetSettingItemCategory : public UUserWidget, public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:
	UWidgetSettingItemCategory(const FObjectInitializer& ObjectInitializer);

public:
	virtual int32 GetLimit_Implementation() const override { return 1000; }

	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation(bool bRecovery) override;

public:
	UFUNCTION(BlueprintPure)
	FText GetCategory() const;

	UFUNCTION(BlueprintCallable)
	void SetCategory(FText InText);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = false))
	UTextBlock* Txt_Category;
};
