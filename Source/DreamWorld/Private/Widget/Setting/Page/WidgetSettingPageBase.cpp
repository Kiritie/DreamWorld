// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetSettingPageBase.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetSettingItemBase.h"
#include "Widget/Setting/Item/WidgetSettingItemCategory.h"

UWidgetSettingPageBase::UWidgetSettingPageBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ParentName = FName("SettingPanel");
	WidgetType = EWidgetType::Temporary;
	WidgetInputMode = EInputMode::UIOnly;
	WidgetCreateType = EWidgetCreateType::AutoCreate;
}

void UWidgetSettingPageBase::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetSettingPageBase::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);
}

void UWidgetSettingPageBase::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);
}

void UWidgetSettingPageBase::OnReset_Implementation()
{
	Super::OnReset_Implementation();
}

void UWidgetSettingPageBase::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

void UWidgetSettingPageBase::OnApply_Implementation()
{
}

bool UWidgetSettingPageBase::CanApply_Implementation() const
{
	return false;
}

bool UWidgetSettingPageBase::CanReset_Implementation() const
{
	return false;
}

void UWidgetSettingPageBase::Apply()
{
	OnApply();
}

void UWidgetSettingPageBase::AddSettingItem_Implementation(UWidgetSettingItemBase* InSettingItem, const FText& InCategory)
{
	if(!InCategory.IsEmpty())
	{
		UWidgetSettingItemCategory* ItemCategory = UObjectPoolModuleBPLibrary::SpawnObject<UWidgetSettingItemCategory>({ InCategory }, UAssetModuleBPLibrary::GetStaticClass(FName("SettingItemCategory")));
		if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ContentBox->AddChild(ItemCategory)))
		{
			ScrollBoxSlot->SetPadding(FMargin(2.5f, 0.f));
		}
	}
	if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ContentBox->AddChild(InSettingItem)))
	{
		ScrollBoxSlot->SetPadding(FMargin(2.5f));
	}
}

void UWidgetSettingPageBase::ClearSettingItems_Implementation()
{
	ContentBox->ClearChildren();
}
