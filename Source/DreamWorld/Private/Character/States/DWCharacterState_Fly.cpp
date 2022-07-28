// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Fly.h"

#include "Ability/Character/States/AbilityCharacterState_Walk.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UDWCharacterState_Fly::UDWCharacterState_Fly()
{
	StateName = FName("Fly");
}

void UDWCharacterState_Fly::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Fly::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Fly);
}

void UDWCharacterState_Fly::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().FlyingTag);

	Character->LimitToAnim();
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Character->GetCharacterMovement()->Velocity = FVector(Character->GetCharacterMovement()->Velocity.X, Character->GetCharacterMovement()->Velocity.Y, 100.f);
	// Character->GetCharacterMovement()->GravityScale = 0.f;
	// Character->GetCharacterMovement()->AirControl = 1.f;
}

void UDWCharacterState_Fly::OnRefresh()
{
	Super::OnRefresh();

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(Character->GetVelocity().Z < 0.f)
	{
		FFindFloorResult FindFloorResult;
		Character->GetCharacterMovement()->FindFloor(Character->GetCharacterMovement()->UpdatedComponent->GetComponentLocation(), FindFloorResult, Character->GetVelocity().IsZero(), nullptr);
		if(FindFloorResult.IsWalkableFloor())
		{
			FSM->SwitchStateByClass<UAbilityCharacterState_Walk>();
		}
	}
}

void UDWCharacterState_Fly::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Fly);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().FlyingTag);

	Character->FreeToAnim();
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	// Character->GetCharacterMovement()->GravityScale = DefaultGravityScale;
	// Character->GetCharacterMovement()->AirControl = DefaultAirControl;
}

void UDWCharacterState_Fly::OnTermination()
{
	Super::OnTermination();
}
