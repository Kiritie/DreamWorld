// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/AbilityModuleTypes.h"
#include "Widget/Screen/UserWidgetBase.h"
#include "WidgetTalentPanel.generated.h"

class UVerticalBox;
class UWidgetTalentContainer;
class UCommonButton;
class UCommonButtonBase;
class UCommonButtonGroup;
class UWidgetTalentItem;
/**
 * ���紴�����
 */
UCLASS()
class DREAMWORLD_API UWidgetTalentPanel : public UUserWidgetBase
{
	GENERATED_BODY()

	friend class UWidgetTalentItem;
	
public:
	UWidgetTalentPanel(const FObjectInitializer& ObjectInitializer);

public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnReset(bool bForce) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

	virtual void OnRefresh() override;

	virtual void OnDestroy(bool bRecovery = false) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnTalentItemSelected(UWidgetTalentItem* InItem);

	UFUNCTION(BlueprintNativeEvent)
	void OnTalentItemDeselected(UWidgetTalentItem* InItem);
	
	UFUNCTION()
	void OnTalentContentRefresh();

	UFUNCTION()
	void OnPreviewContentRefresh();

	UFUNCTION()
	void OnActivateButtonClicked();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UVerticalBox* TalentContent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UPanelWidget* PreviewContent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, OptionalWidget = false), Category = "Components")
	UCommonButton* BtnActivate;
				
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTalentContainer> TalentContainerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetTalentItem> TalentItemClass;

protected:
	UPROPERTY()
	TArray<UWidgetTalentContainer*> TalentContainers;

	UPROPERTY()
	UWidgetTalentItem* SelectedTalentItem;

public:
	UFUNCTION(BlueprintPure)
	bool GetSelectedTalentItem(FAbilityItem& OutItemData) const;
};
