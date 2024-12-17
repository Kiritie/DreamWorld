// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ability/AbilityModuleTypes.h"
#include "Widget/Sub/SubWidgetBase.h"

#include "WidgetAbilityItemCategoryBar.generated.h"

class UCommonButtonBase;
class UCommonButton;
class UCommonTabList;
/**
 * ��ɫ���
 */
UCLASS()
class DREAMWORLD_API UWidgetAbilityItemCategoryBar : public USubWidgetBase
{
	GENERATED_BODY()

public:
	UWidgetAbilityItemCategoryBar(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnInitialize(const TArray<FParameter>& InParams) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnTabButtonCreation(FName TabId, UCommonButtonBase* TabButton);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnTabButtonRemoval(FName TabId, UCommonButtonBase* TabButton);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnTabSelected(FName TabId);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonTabList* CategoryList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	TArray<EAbilityItemType> Categories;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	TSubclassOf<UCommonButton> CategoryClass;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCategorySelected, EAbilityItemType, ItemType);

	UPROPERTY(BlueprintAssignable)
	FOnCategorySelected OnCategorySelected;

public:
	UFUNCTION(BlueprintPure)
	EAbilityItemType GetSelectedItemType() const;

	UFUNCTION(BlueprintCallable)
	void SetSelectedItemType(EAbilityItemType InItemType) const;
};
