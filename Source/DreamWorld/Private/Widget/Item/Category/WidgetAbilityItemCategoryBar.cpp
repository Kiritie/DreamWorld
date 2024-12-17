// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Item/Category/WidgetAbilityItemCategoryBar.h"

#include "Common/CommonStatics.h"
#include "Widget/Common/CommonButton.h"
#include "Widget/Common/CommonTabList.h"

UWidgetAbilityItemCategoryBar::UWidgetAbilityItemCategoryBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CategoryList = nullptr;
	CategoryClass = nullptr;
}

void UWidgetAbilityItemCategoryBar::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	CategoryList->OnTabSelected.AddDynamic(this, &UWidgetAbilityItemCategoryBar::OnTabSelected);
	CategoryList->OnTabButtonCreation.AddDynamic(this, &UWidgetAbilityItemCategoryBar::OnTabButtonCreation);
	CategoryList->OnTabButtonRemoval.AddDynamic(this, &UWidgetAbilityItemCategoryBar::OnTabButtonRemoval);

	for(auto Iter : Categories)
	{
		CategoryList->RegisterTab(*UCommonStatics::GetEnumAuthoredNameByValue(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)Iter), CategoryClass, nullptr);
	}
}

void UWidgetAbilityItemCategoryBar::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetAbilityItemCategoryBar::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetAbilityItemCategoryBar::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);

	OnCategorySelected.Clear();
}

void UWidgetAbilityItemCategoryBar::OnTabButtonCreation_Implementation(FName TabId, UCommonButtonBase* TabButton)
{
	if(UCommonButton* CommonButton = Cast<UCommonButton>(TabButton))
	{
		CommonButton->SetTitle(TabId != TEXT("None") ? UCommonStatics::GetEnumDisplayNameByAuthoredName(TEXT("/Script/WHFramework.EAbilityItemType"), TabId.ToString()) : FText::FromString(TEXT("全部")));
	}
}

void UWidgetAbilityItemCategoryBar::OnTabButtonRemoval_Implementation(FName TabId, UCommonButtonBase* TabButton)
{
}

void UWidgetAbilityItemCategoryBar::OnTabSelected_Implementation(FName TabId)
{
	OnCategorySelected.Broadcast((EAbilityItemType)UCommonStatics::GetEnumValueByAuthoredName(TEXT("/Script/WHFramework.EAbilityItemType"), TabId.ToString()));
}

EAbilityItemType UWidgetAbilityItemCategoryBar::GetSelectedItemType() const
{
	return (EAbilityItemType)UCommonStatics::GetEnumValueByAuthoredName(TEXT("/Script/WHFramework.EAbilityItemType"), CategoryList->GetSelectedTabId().ToString());
}

void UWidgetAbilityItemCategoryBar::SetSelectedItemType(EAbilityItemType InItemType) const
{
	CategoryList->SelectTabByID(*UCommonStatics::GetEnumAuthoredNameByValue(TEXT("/Script/WHFramework.EAbilityItemType"), (int32)InItemType));
}
