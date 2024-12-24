// Fill out your copyright notice in the Description page of Project Settings.

#include "Dialogue/Events/Interact/DWDialogueEvent_DoInteract.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Common/CommonStatics.h"

UDWDialogueEvent_DoInteract::UDWDialogueEvent_DoInteract()
{
	InteractAction = EInteractAction::None;
}

void UDWDialogueEvent_DoInteract::RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	ADWPlayerCharacter* PlayerCharacter = UCommonStatics::GetPlayerPawn<ADWPlayerCharacter>();

	if(!PlayerCharacter) return;

	PlayerCharacter->DoInteract(InteractAction);
}
