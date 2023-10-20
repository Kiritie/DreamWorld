// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetCameraSettingPage.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Camera/CameraModuleBPLibrary.h"
#include "Camera/DWCameraModule.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Setting/Item/WidgetBoolSettingItem.h"
#include "Widget/Setting/Item/WidgetFloatSettingItem.h"

UWidgetCameraSettingPage::UWidgetCameraSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CameraSettingPage");

	Title = FText::FromString(TEXT("摄像机"));
}

void UWidgetCameraSettingPage::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetCameraSettingPage::OnCreate_Implementation(UObject* InOwner)
{
	Super::OnCreate_Implementation(InOwner);

	SettingItem_CameraMoveRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("移动速度")), 100.f, 500.f, 0 }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraMoveRate, FText::FromString(TEXT("移动")));

	SettingItem_SmoothCameraMove = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("平滑移动")) }, UAssetModuleBPLibrary::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_SmoothCameraMove, FText::FromString(TEXT("移动")));

	SettingItem_CameraMoveSpeed = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("平滑速度")), 1.f, 10.f, 0 }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraMoveSpeed, FText::FromString(TEXT("移动")));

	SettingItem_ReverseCameraPitch = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("垂直反转")) }, UAssetModuleBPLibrary::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_ReverseCameraPitch, FText::FromString(TEXT("旋转")));

	SettingItem_CameraTurnRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("水平速度")), 30.f, 150.f, 0 }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraTurnRate, FText::FromString(TEXT("旋转")));

	SettingItem_CameraLookUpRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("垂直速度")), 30.f, 150.f, 0 }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraLookUpRate, FText::FromString(TEXT("旋转")));

	SettingItem_SmoothCameraRotate = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("平滑旋转")) }, UAssetModuleBPLibrary::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_SmoothCameraRotate, FText::FromString(TEXT("旋转")));

	SettingItem_CameraRotateSpeed = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("平滑速度")), 1.f, 10.f, 0 }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraRotateSpeed, FText::FromString(TEXT("旋转")));

	SettingItem_CameraZoomRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("缩放速度")), 50.f, 300.f, 0 }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraZoomRate, FText::FromString(TEXT("缩放")));

	SettingItem_SmoothCameraZoom = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("平滑缩放")) }, UAssetModuleBPLibrary::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_SmoothCameraZoom, FText::FromString(TEXT("缩放")));

	SettingItem_CameraZoomSpeed = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("平滑速度")), 1.f, 10.f, 0 }, UAssetModuleBPLibrary::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraZoomSpeed, FText::FromString(TEXT("缩放")));
}

void UWidgetCameraSettingPage::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	SettingItem_CameraMoveRate->SetValue(ADWCameraModule::Get()->GetCameraMoveRate());
	SettingItem_SmoothCameraMove->SetValue(ADWCameraModule::Get()->IsSmoothCameraMove());
	SettingItem_CameraMoveSpeed->SetValue(ADWCameraModule::Get()->GetCameraMoveSpeed());
	SettingItem_ReverseCameraPitch->SetValue(ADWCameraModule::Get()->IsReverseCameraPitch());
	SettingItem_CameraTurnRate->SetValue(ADWCameraModule::Get()->GetCameraTurnRate());
	SettingItem_CameraLookUpRate->SetValue(ADWCameraModule::Get()->GetCameraLookUpRate());
	SettingItem_SmoothCameraRotate->SetValue(ADWCameraModule::Get()->IsSmoothCameraRotate());
	SettingItem_CameraRotateSpeed->SetValue(ADWCameraModule::Get()->GetCameraRotateSpeed());
	SettingItem_CameraZoomRate->SetValue(ADWCameraModule::Get()->GetCameraZoomRate());
	SettingItem_SmoothCameraZoom->SetValue(ADWCameraModule::Get()->IsSmoothCameraZoom());
	SettingItem_CameraZoomSpeed->SetValue(ADWCameraModule::Get()->GetCameraZoomSpeed());
}

void UWidgetCameraSettingPage::OnApply_Implementation()
{
	Super::OnApply_Implementation();

	ADWCameraModule::Get()->SetCameraMoveRate(SettingItem_CameraMoveRate->GetValue().GetFloatValue());
	ADWCameraModule::Get()->SetSmoothCameraMove(SettingItem_SmoothCameraMove->GetValue().GetBooleanValue());
	ADWCameraModule::Get()->SetCameraMoveSpeed(SettingItem_CameraMoveSpeed->GetValue().GetFloatValue());
	ADWCameraModule::Get()->SetReverseCameraPitch(SettingItem_ReverseCameraPitch->GetValue().GetBooleanValue());
	ADWCameraModule::Get()->SetCameraTurnRate(SettingItem_CameraTurnRate->GetValue().GetFloatValue());
	ADWCameraModule::Get()->SetCameraLookUpRate(SettingItem_CameraLookUpRate->GetValue().GetFloatValue());
	ADWCameraModule::Get()->SetSmoothCameraRotate(SettingItem_SmoothCameraRotate->GetValue().GetBooleanValue());
	ADWCameraModule::Get()->SetCameraRotateSpeed(SettingItem_CameraRotateSpeed->GetValue().GetFloatValue());
	ADWCameraModule::Get()->SetCameraZoomRate(SettingItem_CameraZoomRate->GetValue().GetFloatValue());
	ADWCameraModule::Get()->SetSmoothCameraZoom(SettingItem_SmoothCameraZoom->GetValue().GetBooleanValue());
	ADWCameraModule::Get()->SetCameraZoomSpeed(SettingItem_CameraZoomSpeed->GetValue().GetFloatValue());
}

void UWidgetCameraSettingPage::OnReset_Implementation()
{
	Super::OnReset_Implementation();

	SettingItem_CameraMoveRate->SetValue(GetDefaultCameraData().CameraMoveRate);
	SettingItem_SmoothCameraMove->SetValue(GetDefaultCameraData().bSmoothCameraMove);
	SettingItem_CameraMoveSpeed->SetValue(GetDefaultCameraData().CameraMoveSpeed);
	SettingItem_ReverseCameraPitch->SetValue(GetDefaultCameraData().bReverseCameraPitch);
	SettingItem_CameraTurnRate->SetValue(GetDefaultCameraData().CameraTurnRate);
	SettingItem_CameraLookUpRate->SetValue(GetDefaultCameraData().CameraLookUpRate);
	SettingItem_SmoothCameraRotate->SetValue(GetDefaultCameraData().bSmoothCameraRotate);
	SettingItem_CameraRotateSpeed->SetValue(GetDefaultCameraData().CameraRotateSpeed);
	SettingItem_CameraZoomRate->SetValue(GetDefaultCameraData().CameraZoomRate);
	SettingItem_SmoothCameraZoom->SetValue(GetDefaultCameraData().bSmoothCameraZoom);
	SettingItem_CameraZoomSpeed->SetValue(GetDefaultCameraData().CameraZoomSpeed);
}

void UWidgetCameraSettingPage::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);
}

bool UWidgetCameraSettingPage::CanApply_Implementation() const
{
	return ADWCameraModule::Get()->GetCameraMoveRate() != SettingItem_CameraMoveRate->GetValue().GetFloatValue() ||
		ADWCameraModule::Get()->IsSmoothCameraMove() != SettingItem_SmoothCameraMove->GetValue().GetBooleanValue() ||
		ADWCameraModule::Get()->GetCameraMoveSpeed() != SettingItem_CameraMoveSpeed->GetValue().GetFloatValue() ||
		ADWCameraModule::Get()->IsReverseCameraPitch() != SettingItem_ReverseCameraPitch->GetValue().GetBooleanValue() ||
		ADWCameraModule::Get()->GetCameraTurnRate() != SettingItem_CameraTurnRate->GetValue().GetFloatValue() ||
		ADWCameraModule::Get()->GetCameraLookUpRate() != SettingItem_CameraLookUpRate->GetValue().GetFloatValue() ||
		ADWCameraModule::Get()->IsSmoothCameraRotate() != SettingItem_SmoothCameraRotate->GetValue().GetBooleanValue() ||
		ADWCameraModule::Get()->GetCameraRotateSpeed() != SettingItem_CameraRotateSpeed->GetValue().GetFloatValue() ||
		ADWCameraModule::Get()->GetCameraZoomRate() != SettingItem_CameraZoomRate->GetValue().GetFloatValue() ||
		ADWCameraModule::Get()->IsSmoothCameraZoom() != SettingItem_SmoothCameraZoom->GetValue().GetBooleanValue() ||
		ADWCameraModule::Get()->GetCameraZoomSpeed() != SettingItem_CameraZoomSpeed->GetValue().GetFloatValue();
}

bool UWidgetCameraSettingPage::CanReset_Implementation() const
{
	return ADWCameraModule::Get()->GetCameraMoveRate() != GetDefaultCameraData().CameraMoveRate ||
		ADWCameraModule::Get()->IsSmoothCameraMove() != GetDefaultCameraData().bSmoothCameraMove ||
		ADWCameraModule::Get()->GetCameraMoveSpeed() != GetDefaultCameraData().CameraMoveSpeed ||
		ADWCameraModule::Get()->IsReverseCameraPitch() != GetDefaultCameraData().bReverseCameraPitch ||
		ADWCameraModule::Get()->GetCameraTurnRate() != GetDefaultCameraData().CameraTurnRate ||
		ADWCameraModule::Get()->GetCameraLookUpRate() != GetDefaultCameraData().CameraLookUpRate ||
		ADWCameraModule::Get()->IsSmoothCameraRotate() != GetDefaultCameraData().bSmoothCameraRotate ||
		ADWCameraModule::Get()->GetCameraRotateSpeed() != GetDefaultCameraData().CameraRotateSpeed ||
		ADWCameraModule::Get()->GetCameraZoomRate() != GetDefaultCameraData().CameraZoomRate ||
		ADWCameraModule::Get()->IsSmoothCameraZoom() != GetDefaultCameraData().bSmoothCameraZoom ||
		ADWCameraModule::Get()->GetCameraZoomSpeed() != GetDefaultCameraData().CameraZoomSpeed;
}

FDWCameraModuleSaveData& UWidgetCameraSettingPage::GetDefaultCameraData() const
{
	return USaveGameModuleBPLibrary::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().CameraData;
}
