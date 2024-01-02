// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Common/DWCommonTypes.h"
#include "Setting/Widget/Page/Module/WidgetInputSettingPageBase.h"

#include "WidgetInputSettingPage.generated.h"

class UWidgetKeySettingItem;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetInputSettingPage : public UWidgetInputSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetInputSettingPage(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FSaveData* GetDefaultSaveData() const override;
};
