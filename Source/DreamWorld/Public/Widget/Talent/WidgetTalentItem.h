// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widget/Item/WidgetAbilityItem.h"

#include "WidgetTalentItem.generated.h"

class UWidgetTalentLine;
class UWidgetTalentContainer;
/**
 * ���紴�����
 */
UCLASS()
class DREAMWORLD_API UWidgetTalentItem : public UWidgetAbilityItem
{
	GENERATED_BODY()
	
public:
	UWidgetTalentItem(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery) override;

public:
	virtual void NativeOnSelected(bool bBroadcast) override;

	virtual void NativeOnDeselected(bool bBroadcast) override;

public:
	virtual void OnAddToContainer(UWidgetTalentContainer* InTalentContainer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UTextBlock* TxtLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UImage* ImgError;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTalentLine> TalentLineClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FAbilityItem ParentItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetTalentLine* TalentLine;

public:
	FAbilityItem GetParentItem() const { return ParentItem; }
};
