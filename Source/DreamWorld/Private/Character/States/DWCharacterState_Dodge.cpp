// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Dodge.h"

#include "Character/DWCharacter.h"
#include "Components/CapsuleComponent.h"

UDWCharacterState_Dodge::UDWCharacterState_Dodge()
{
	StateName = FName("Dodge");
}

void UDWCharacterState_Dodge::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Dodge::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Dodge);
}

void UDWCharacterState_Dodge::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->LimitToAnim();
	Character->GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	Character->SetActorRotation(FRotator(0.f, Character->MoveDirection.ToOrientationRotator().Yaw, 0.f));
}

void UDWCharacterState_Dodge::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Dodge::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Dodge);

	Character->FreeToAnim();
	Character->GetCapsuleComponent()->SetGenerateOverlapEvents(true);
}

void UDWCharacterState_Dodge::OnTermination()
{
	Super::OnTermination();
}
