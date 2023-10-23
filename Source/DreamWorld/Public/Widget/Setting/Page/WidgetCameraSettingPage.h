// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetSettingPageBase.h"
#include "Common/DWCommonTypes.h"

#include "WidgetCameraSettingPage.generated.h"

class UWidgetBoolSettingItem;
class UWidgetFloatSettingItem;

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetCameraSettingPage : public UWidgetSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetCameraSettingPage(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnInitialize(UObject* InOwner) override;

	virtual void OnCreate(UObject* InOwner) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnApply() override;

	virtual void OnReset() override;

	virtual void OnClose(bool bInstant) override;

public:
	virtual bool CanApply_Implementation() const override;

	virtual bool CanReset_Implementation() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItem* SettingItem_CameraMoveRate;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetBoolSettingItem* SettingItem_SmoothCameraMove;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItem* SettingItem_CameraMoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetBoolSettingItem* SettingItem_ReverseCameraPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItem* SettingItem_CameraTurnRate;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItem* SettingItem_CameraLookUpRate;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetBoolSettingItem* SettingItem_SmoothCameraRotate;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItem* SettingItem_CameraRotateSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItem* SettingItem_CameraZoomRate;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetBoolSettingItem* SettingItem_SmoothCameraZoom;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWidgetFloatSettingItem* SettingItem_CameraZoomSpeed;

public:
	UFUNCTION(BlueprintPure)
	FDWCameraModuleSaveData& GetDefaultCameraData() const;
};
