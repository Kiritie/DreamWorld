// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Death.h"

#include "AchievementSubSystem.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/WHGameInstance.h"
#include "Common/CommonBPLibrary.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Common/Targeting/TargetingComponent.h"

UDWPlayerCharacterState_Death::UDWPlayerCharacterState_Death()
{
	
}

void UDWPlayerCharacterState_Death::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWPlayerCharacterState_Death::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWPlayerCharacterState_Death::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	PlayerCharacter->GetTargeting()->TargetLockOff();

	PlayerCharacter->GetInteractionComponent()->SetInteractable(false);
	
	UCommonBPLibrary::GetGameInstance()->GetSubsystem<UAchievementSubSystem>()->Unlock(FName("FirstDeath"));
}

void UDWPlayerCharacterState_Death::OnRefresh()
{
	Super::OnRefresh();
}

void UDWPlayerCharacterState_Death::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	PlayerCharacter->SetInteractingAgent(nullptr);
	
	PlayerCharacter->GetInteractionComponent()->SetInteractable(true);
}

void UDWPlayerCharacterState_Death::OnTermination()
{
	Super::OnTermination();
}

void UDWPlayerCharacterState_Death::DeathStart()
{
	Super::DeathStart();
}

void UDWPlayerCharacterState_Death::DeathEnd()
{
	Super::DeathEnd();

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	PlayerCharacter->SetInteractingAgent(PlayerCharacter);
}
