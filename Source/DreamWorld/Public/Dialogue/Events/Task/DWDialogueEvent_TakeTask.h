// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "DWDialogueEvent_TakeTask.generated.h"

class UTaskAsset;
/**
 * 
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UDWDialogueEvent_TakeTask : public UDialogueEvents
{
	GENERATED_BODY()

public:
	UDWDialogueEvent_TakeTask();

public:
	virtual void RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTaskAsset* TaskAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TaskGUID;
};
