// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Setting/Widget/Page/Module/WidgetWidgetSettingPageBase.h"

#include "WidgetWidgetSettingPage.generated.h"

class UWidgetFloatSettingItem;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetWidgetSettingPage : public UWidgetWidgetSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetWidgetSettingPage(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FSaveData* GetDefaultSaveData() const override;
};
