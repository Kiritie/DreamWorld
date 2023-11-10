// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Setting/Widget/Page/WidgetAudioSettingPageBase.h"

#include "WidgetAudioSettingPage.generated.h"

class UWidgetFloatSettingItem;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetAudioSettingPage : public UWidgetAudioSettingPageBase
{
	GENERATED_BODY()
	
public:
	UWidgetAudioSettingPage(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FSaveData* GetDefaultSaveData() const override;
};
