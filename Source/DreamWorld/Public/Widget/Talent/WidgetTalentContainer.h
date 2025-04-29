// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Sub/SubWidgetBase.h"

#include "WidgetTalentContainer.generated.h"

class UHorizontalBox;
class UWidgetTalentItem;
/**
 * UI构建项
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UWidgetTalentContainer : public USubWidgetBase
{
	GENERATED_BODY()
	
public:
	UWidgetTalentContainer(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery) override;

public:
	UFUNCTION(Blueprintable)
	void AddTalentItem(UWidgetTalentItem* InTalentItem);

	UFUNCTION(Blueprintable)
	void ClearTalentItem();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UHorizontalBox* TalentContent;

protected:
	UPROPERTY()
	TArray<UWidgetTalentItem*> TalentItems;

public:
	TArray<UWidgetTalentItem*> GetTalentItems() const { return TalentItems; }
	
	UHorizontalBox* GetTalentContent() const { return TalentContent; }
};
