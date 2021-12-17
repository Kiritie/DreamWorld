// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Blueprint/UserWidget.h"
#include "WidgetWorldText.generated.h"

/**
 * �����ı�
 */
UCLASS()
class DREAMWORLD_API UWidgetWorldText : public UUserWidget
{
	GENERATED_BODY()

public:
	UWidgetWorldText(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitContent(const FString& InContent, EWorldTextType InContentType);
};
