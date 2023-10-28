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

void UWidgetSettingPageBase::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetSettingPageBase::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);
}

void UWidgetSettingPageBase::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetSettingPageBase::OnReset()
{
	Super::OnReset();
}

void UWidgetSettingPageBase::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetSettingPageBase::OnApply()
{
	K2_OnApply();
}

void UWidgetSettingPageBase::OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	K2_OnValueChange(InSettingItem, InValue);
}

void UWidgetSettingPageBase::OnValuesChange(UWidgetSettingItemBase* InSettingItem, const TArray<FParameter>& InValues)
{
	K2_OnValuesChange(InSettingItem, InValues);
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
	if(SettingItems.Contains(InSettingItem)) return;
	
	InSettingItem->OnValueChanged.RemoveAll(this);
	InSettingItem->OnValueChanged.AddDynamic(this, &UWidgetSettingPageBase::OnValueChange);
	InSettingItem->OnValuesChanged.RemoveAll(this);
	InSettingItem->OnValuesChanged.AddDynamic(this, &UWidgetSettingPageBase::OnValuesChange);
	if(!InCategory.IsEmpty() && !InCategory.EqualTo(LastCategory))
	{
		LastCategory = InCategory;
		if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ContentBox->AddChild(CreateSubWidget<UWidgetSettingItemCategory>({ InCategory }, UAssetModuleBPLibrary::GetStaticClass(FName("SettingItemCategory"))))))
		{
			ScrollBoxSlot->SetPadding(FMargin(2.5f, 0.f));
		}
	}
	if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ContentBox->AddChild(InSettingItem)))
	{
		ScrollBoxSlot->SetPadding(FMargin(2.5f));
	}
	SettingItems.Add(InSettingItem);
}

void UWidgetSettingPageBase::ClearSettingItems_Implementation()
{
	for(auto Iter : ContentBox->GetAllChildren())
	{
		if(UWidgetSettingItemBase* SettingItem = Cast<UWidgetSettingItemBase>(Iter))
		{
			SettingItem->Destroy(true);
		}
	}
	ContentBox->ClearChildren();
	SettingItems.Empty();
}
