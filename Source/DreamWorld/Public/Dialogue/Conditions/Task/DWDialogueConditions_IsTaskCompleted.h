// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "DWDialogueConditions_IsTaskCompleted.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWDialogueConditions_IsTaskCompleted : public UDialogueConditions
{
	GENERATED_BODY()

public:
	UDWDialogueConditions_IsTaskCompleted();

public:
	virtual bool IsConditionMet_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TaskGUID;
};
