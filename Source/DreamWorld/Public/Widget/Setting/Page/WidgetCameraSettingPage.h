// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Setting/Widget/Page/Module/WidgetCameraSettingPageBase.h"

#include "WidgetCameraSettingPage.generated.h"

class UWidgetBoolSettingItem;
class UWidgetFloatSettingItem;

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetCameraSettingPage : public UWidgetCameraSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetCameraSettingPage(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FSaveData* GetDefaultSaveData() const override;
};
