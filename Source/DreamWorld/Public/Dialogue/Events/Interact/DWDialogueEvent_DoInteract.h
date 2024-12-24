// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Common/CommonTypes.h"
#include "DWDialogueEvent_DoInteract.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UDWDialogueEvent_DoInteract : public UDialogueEvents
{
	GENERATED_BODY()

public:
	UDWDialogueEvent_DoInteract();

public:
	virtual void RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EInteractAction InteractAction;
};
