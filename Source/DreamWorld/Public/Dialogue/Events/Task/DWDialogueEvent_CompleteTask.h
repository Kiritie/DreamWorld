// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Task/TaskModuleTypes.h"
#include "DWDialogueEvent_CompleteTask.generated.h"

class UTaskAsset;
/**
 * 
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UDWDialogueEvent_CompleteTask : public UDialogueEvents
{
	GENERATED_BODY()

public:
	UDWDialogueEvent_CompleteTask();

public:
	virtual void RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTaskAsset* TaskAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TaskGUID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETaskExecuteResult TaskResult;
};
