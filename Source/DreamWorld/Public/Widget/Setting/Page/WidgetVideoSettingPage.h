// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Setting/Widget/Page/WidgetVideoSettingPageBase.h"

#include "WidgetVideoSettingPage.generated.h"

class UWidgetEnumSettingItem;

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetVideoSettingPage : public UWidgetVideoSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetVideoSettingPage(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FSaveData* GetDefaultSaveData() const override;
};
