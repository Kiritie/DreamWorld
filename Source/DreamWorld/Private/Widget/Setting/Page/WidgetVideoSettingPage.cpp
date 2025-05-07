// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetVideoSettingPage.h"

#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Setting/SettingModule.h"
#include "Setting/Widget/Item/WidgetFloatSettingItemBase.h"
#include "Voxel/VoxelModule.h"

UWidgetVideoSettingPage::UWidgetVideoSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Title = FText::FromString(TEXT("视频"));
}

void UWidgetVideoSettingPage::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	SettingItem_VoxelWorldRange = UObjectPoolModuleStatics::SpawnObject<UWidgetFloatSettingItemBase>(nullptr, { FText::FromString(TEXT("世界渲染距离")), 5.f, 50.f, 0 }, USettingModule::Get().GetFloatSettingItemClass());
	SettingItem_VoxelWorldRange->SetValue(UVoxelModule::Get().GetWorldData().WorldRange.X);
	AddSettingItem(FName("VoxelWorldRange"), SettingItem_VoxelWorldRange, FText::FromString(TEXT("体素")));
}

void UWidgetVideoSettingPage::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	SettingItem_VoxelWorldRange->SetValue(GetDefaultSaveData()->CastRef<FDWVideoModuleSaveData>().VoxelWorldRange);
}

void UWidgetVideoSettingPage::OnApply()
{
	Super::OnApply();

	UVoxelModule::Get().GetWorldData().WorldRange = FVector2D(SettingItem_VoxelWorldRange->GetValue().GetFloatValue());
	UVoxelModule::Get().GenerateChunkQueues(true, true);
}

void UWidgetVideoSettingPage::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UWidgetVideoSettingPage::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	SettingItem_VoxelWorldRange->SetValue(UVoxelModule::Get().GetWorldData().WorldRange.X);
}

void UWidgetVideoSettingPage::OnValueChange(UWidgetSettingItemBase* InSettingItem, const FParameter& InValue)
{
	Super::OnValueChange(InSettingItem, InValue);
}

bool UWidgetVideoSettingPage::CanApply_Implementation() const
{
	return Super::CanApply_Implementation() || UVoxelModule::Get().GetWorldData().WorldRange.X != SettingItem_VoxelWorldRange->GetValue().GetFloatValue();
}

bool UWidgetVideoSettingPage::CanReset_Implementation() const
{
	return Super::CanReset_Implementation() || UVoxelModule::Get().GetWorldData().WorldRange.X != GetDefaultSaveData()->CastRef<FDWVideoModuleSaveData>().VoxelWorldRange;
}

FSaveData* UWidgetVideoSettingPage::GetDefaultSaveData() const
{
	return &USaveGameModuleStatics::GetOrCreateSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingModuleSaveData>().VideoData;
}
