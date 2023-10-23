// Fill out your copyright notice in the Description Item of Project Settings.


#include "Widget/Setting/Item/WidgetSettingItemCategory.h"

#include "Components/TextBlock.h"
#include "Widget/WidgetModuleBPLibrary.h"

UWidgetSettingItemCategory::UWidgetSettingItemCategory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetSettingItemCategory::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void UWidgetSettingItemCategory::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
	
	SetCategory(FText::GetEmpty());
}

void UWidgetSettingItemCategory::OnCreate(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetSettingItemCategory::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);

	if(InParams.IsValidIndex(0))
	{
		SetCategory(InParams[0].GetTextValue());
	}
}

void UWidgetSettingItemCategory::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetSettingItemCategory::OnDestroy()
{
	Super::OnDestroy();
}

FText UWidgetSettingItemCategory::GetCategory() const
{
	return Txt_Category->GetText();
}

void UWidgetSettingItemCategory::SetCategory(FText InText)
{
	Txt_Category->SetText(InText);
}
