// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"

#include "WidgetButtonBase.generated.h"

class UObject;
struct FFrame;

UCLASS(Abstract, BlueprintType, Blueprintable)
class UWidgetButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonText;

public:
	FText GetButtonText() const { return ButtonText; }

	void SetButtonText(const FText& InButtonText) { ButtonText = InButtonText; }
};
