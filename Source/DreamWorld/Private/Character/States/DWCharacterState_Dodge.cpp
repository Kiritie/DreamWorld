// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Dodge.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Components/CapsuleComponent.h"

UDWCharacterState_Dodge::UDWCharacterState_Dodge()
{
	StateName = FName("Dodge");
}

void UDWCharacterState_Dodge::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Dodge::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->GetMoveDirection(true) != FVector::ZeroVector && Character->DoAction(GameplayTags::Ability_Character_Action_Dodge);
}

void UDWCharacterState_Dodge::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Dodging);

	Character->LimitToAnim();
	Character->GetCollisionComponent()->SetGenerateOverlapEvents(false);
	Character->SetActorRotation(FRotator(0.f, Character->GetMoveDirection(true).ToOrientationRotator().Yaw, 0.f));
}

void UDWCharacterState_Dodge::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Dodge::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::Ability_Character_Action_Dodge);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Dodging);

	Character->FreeToAnim();
	Character->GetCollisionComponent()->SetGenerateOverlapEvents(true);
}

void UDWCharacterState_Dodge::OnTermination()
{
	Super::OnTermination();
}
