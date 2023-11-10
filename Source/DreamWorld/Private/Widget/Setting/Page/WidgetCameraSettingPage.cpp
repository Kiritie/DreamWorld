// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Setting/Page/WidgetCameraSettingPage.h"

#include "Asset/AssetModuleStatics.h"
#include "Camera/CameraModuleStatics.h"
#include "Camera/DWCameraModule.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "SaveGame/Setting/DWSettingSaveGame.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Setting/Item/WidgetBoolSettingItem.h"
#include "Widget/Setting/Item/WidgetFloatSettingItem.h"

UWidgetCameraSettingPage::UWidgetCameraSettingPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("CameraSettingPage");

	Title = FText::FromString(TEXT("摄像机"));
}

void UWidgetCameraSettingPage::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetCameraSettingPage::OnCreate(UObject* InOwner)
{
	Super::OnCreate(InOwner);

	SettingItem_CameraMoveRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("移动速度")), 100.f, 500.f, 0 }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraMoveRate, FText::FromString(TEXT("移动")));

	SettingItem_SmoothCameraMove = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("平滑移动")) }, UAssetModuleStatics::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_SmoothCameraMove, FText::FromString(TEXT("移动")));

	SettingItem_CameraMoveSpeed = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("平滑速度")), 1.f, 10.f, 0 }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraMoveSpeed, FText::FromString(TEXT("移动")));

	SettingItem_ReverseCameraPitch = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("垂直反转")) }, UAssetModuleStatics::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_ReverseCameraPitch, FText::FromString(TEXT("旋转")));

	SettingItem_CameraTurnRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("水平速度")), 30.f, 150.f, 0 }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraTurnRate, FText::FromString(TEXT("旋转")));

	SettingItem_CameraLookUpRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("垂直速度")), 30.f, 150.f, 0 }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraLookUpRate, FText::FromString(TEXT("旋转")));

	SettingItem_SmoothCameraRotate = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("平滑旋转")) }, UAssetModuleStatics::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_SmoothCameraRotate, FText::FromString(TEXT("旋转")));

	SettingItem_CameraRotateSpeed = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("平滑速度")), 1.f, 10.f, 0 }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraRotateSpeed, FText::FromString(TEXT("旋转")));

	SettingItem_CameraZoomRate = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("缩放速度")), 50.f, 300.f, 0 }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraZoomRate, FText::FromString(TEXT("缩放")));

	SettingItem_SmoothCameraZoom = CreateSubWidget<UWidgetBoolSettingItem>({ FText::FromString(TEXT("平滑缩放")) }, UAssetModuleStatics::GetStaticClass(FName("BoolSettingItem")));
	AddSettingItem(SettingItem_SmoothCameraZoom, FText::FromString(TEXT("缩放")));

	SettingItem_CameraZoomSpeed = CreateSubWidget<UWidgetFloatSettingItem>({ FText::FromString(TEXT("平滑速度")), 1.f, 10.f, 0 }, UAssetModuleStatics::GetStaticClass(FName("FloatSettingItem")));
	AddSettingItem(SettingItem_CameraZoomSpeed, FText::FromString(TEXT("缩放")));
}

void UWidgetCameraSettingPage::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	SettingItem_CameraMoveRate->SetValue(UDWCameraModule::Get().GetCameraMoveRate());
	SettingItem_SmoothCameraMove->SetValue(UDWCameraModule::Get().IsSmoothCameraMove());
	SettingItem_CameraMoveSpeed->SetValue(UDWCameraModule::Get().GetCameraMoveSpeed());
	SettingItem_ReverseCameraPitch->SetValue(UDWCameraModule::Get().IsReverseCameraPitch());
	SettingItem_CameraTurnRate->SetValue(UDWCameraModule::Get().GetCameraTurnRate());
	SettingItem_CameraLookUpRate->SetValue(UDWCameraModule::Get().GetCameraLookUpRate());
	SettingItem_SmoothCameraRotate->SetValue(UDWCameraModule::Get().IsSmoothCameraRotate());
	SettingItem_CameraRotateSpeed->SetValue(UDWCameraModule::Get().GetCameraRotateSpeed());
	SettingItem_CameraZoomRate->SetValue(UDWCameraModule::Get().GetCameraZoomRate());
	SettingItem_SmoothCameraZoom->SetValue(UDWCameraModule::Get().IsSmoothCameraZoom());
	SettingItem_CameraZoomSpeed->SetValue(UDWCameraModule::Get().GetCameraZoomSpeed());
}

void UWidgetCameraSettingPage::OnApply()
{
	Super::OnApply();

	UDWCameraModule::Get().SetCameraMoveRate(SettingItem_CameraMoveRate->GetValue().GetFloatValue());
	UDWCameraModule::Get().SetSmoothCameraMove(SettingItem_SmoothCameraMove->GetValue().GetBooleanValue());
	UDWCameraModule::Get().SetCameraMoveSpeed(SettingItem_CameraMoveSpeed->GetValue().GetFloatValue());
	UDWCameraModule::Get().SetReverseCameraPitch(SettingItem_ReverseCameraPitch->GetValue().GetBooleanValue());
	UDWCameraModule::Get().SetCameraTurnRate(SettingItem_CameraTurnRate->GetValue().GetFloatValue());
	UDWCameraModule::Get().SetCameraLookUpRate(SettingItem_CameraLookUpRate->GetValue().GetFloatValue());
	UDWCameraModule::Get().SetSmoothCameraRotate(SettingItem_SmoothCameraRotate->GetValue().GetBooleanValue());
	UDWCameraModule::Get().SetCameraRotateSpeed(SettingItem_CameraRotateSpeed->GetValue().GetFloatValue());
	UDWCameraModule::Get().SetCameraZoomRate(SettingItem_CameraZoomRate->GetValue().GetFloatValue());
	UDWCameraModule::Get().SetSmoothCameraZoom(SettingItem_SmoothCameraZoom->GetValue().GetBooleanValue());
	UDWCameraModule::Get().SetCameraZoomSpeed(SettingItem_CameraZoomSpeed->GetValue().GetFloatValue());
}

void UWidgetCameraSettingPage::OnReset()
{
	Super::OnReset();

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

void UWidgetCameraSettingPage::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

bool UWidgetCameraSettingPage::CanApply_Implementation() const
{
	return UDWCameraModule::Get().GetCameraMoveRate() != SettingItem_CameraMoveRate->GetValue().GetFloatValue() ||
		UDWCameraModule::Get().IsSmoothCameraMove() != SettingItem_SmoothCameraMove->GetValue().GetBooleanValue() ||
		UDWCameraModule::Get().GetCameraMoveSpeed() != SettingItem_CameraMoveSpeed->GetValue().GetFloatValue() ||
		UDWCameraModule::Get().IsReverseCameraPitch() != SettingItem_ReverseCameraPitch->GetValue().GetBooleanValue() ||
		UDWCameraModule::Get().GetCameraTurnRate() != SettingItem_CameraTurnRate->GetValue().GetFloatValue() ||
		UDWCameraModule::Get().GetCameraLookUpRate() != SettingItem_CameraLookUpRate->GetValue().GetFloatValue() ||
		UDWCameraModule::Get().IsSmoothCameraRotate() != SettingItem_SmoothCameraRotate->GetValue().GetBooleanValue() ||
		UDWCameraModule::Get().GetCameraRotateSpeed() != SettingItem_CameraRotateSpeed->GetValue().GetFloatValue() ||
		UDWCameraModule::Get().GetCameraZoomRate() != SettingItem_CameraZoomRate->GetValue().GetFloatValue() ||
		UDWCameraModule::Get().IsSmoothCameraZoom() != SettingItem_SmoothCameraZoom->GetValue().GetBooleanValue() ||
		UDWCameraModule::Get().GetCameraZoomSpeed() != SettingItem_CameraZoomSpeed->GetValue().GetFloatValue();
}

bool UWidgetCameraSettingPage::CanReset_Implementation() const
{
	return UDWCameraModule::Get().GetCameraMoveRate() != GetDefaultCameraData().CameraMoveRate ||
		UDWCameraModule::Get().IsSmoothCameraMove() != GetDefaultCameraData().bSmoothCameraMove ||
		UDWCameraModule::Get().GetCameraMoveSpeed() != GetDefaultCameraData().CameraMoveSpeed ||
		UDWCameraModule::Get().IsReverseCameraPitch() != GetDefaultCameraData().bReverseCameraPitch ||
		UDWCameraModule::Get().GetCameraTurnRate() != GetDefaultCameraData().CameraTurnRate ||
		UDWCameraModule::Get().GetCameraLookUpRate() != GetDefaultCameraData().CameraLookUpRate ||
		UDWCameraModule::Get().IsSmoothCameraRotate() != GetDefaultCameraData().bSmoothCameraRotate ||
		UDWCameraModule::Get().GetCameraRotateSpeed() != GetDefaultCameraData().CameraRotateSpeed ||
		UDWCameraModule::Get().GetCameraZoomRate() != GetDefaultCameraData().CameraZoomRate ||
		UDWCameraModule::Get().IsSmoothCameraZoom() != GetDefaultCameraData().bSmoothCameraZoom ||
		UDWCameraModule::Get().GetCameraZoomSpeed() != GetDefaultCameraData().CameraZoomSpeed;
}

FDWCameraModuleSaveData& UWidgetCameraSettingPage::GetDefaultCameraData() const
{
	return USaveGameModuleStatics::GetSaveGame<UDWSettingSaveGame>()->GetDefaultDataRef<FDWSettingSaveData>().CameraData;
}
