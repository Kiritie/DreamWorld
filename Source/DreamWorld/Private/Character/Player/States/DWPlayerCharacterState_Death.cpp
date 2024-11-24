// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Death.h"

#include "Achievement/AchievementModuleStatics.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Common/Targeting/TargetingComponent.h"
#include "Input/InputModule.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleStatics.h"

UDWPlayerCharacterState_Death::UDWPlayerCharacterState_Death()
{
	
}

void UDWPlayerCharacterState_Death::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWPlayerCharacterState_Death::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	return Super::OnPreEnter(InLastState, InParams);
}

void UDWPlayerCharacterState_Death::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	PlayerCharacter->GetTargeting()->TargetLockOff();

	UAchievementModuleStatics::UnlockAchievement(FName("FirstDeath"));
	
	UInputModule::Get().Reset();
	
	if(UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->GetTemporaryChild())
	{
		UWidgetModuleStatics::GetUserWidget<UWidgetGameHUD>()->GetTemporaryChild()->Close();
	}
}

void UDWPlayerCharacterState_Death::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWPlayerCharacterState_Death::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	PlayerCharacter->SetInteractingAgent(nullptr, true);
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

	PlayerCharacter->SetInteractingAgent(PlayerCharacter, true);
}
