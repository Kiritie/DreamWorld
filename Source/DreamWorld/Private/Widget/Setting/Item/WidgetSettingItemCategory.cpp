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

void UWidgetSettingItemCategory::OnCreate_Implementation(UUserWidgetBase* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate_Implementation(InOwner, InParams);
}

void UWidgetSettingItemCategory::OnInitialize_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnInitialize_Implementation(InParams);

	if(InParams.IsValidIndex(0))
	{
		SetCategory(InParams[0].GetTextValue());
	}
}

void UWidgetSettingItemCategory::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}

void UWidgetSettingItemCategory::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}

FText UWidgetSettingItemCategory::GetCategory() const
{
	return Txt_Category->GetText();
}

void UWidgetSettingItemCategory::SetCategory(FText InText)
{
	Txt_Category->SetText(InText);
}
