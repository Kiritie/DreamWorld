// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Fly.h"

#include "Ability/Character/States/AbilityCharacterState_Walk.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Scene/SceneModuleStatics.h"

UDWCharacterState_Fly::UDWCharacterState_Fly()
{
	StateName = FName("Fly");
}

void UDWCharacterState_Fly::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Fly::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnEnterValidate(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Fly);
}

void UDWCharacterState_Fly::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Flying);

	Character->LimitToAnim();
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Character->GetCharacterMovement()->Velocity.Z = 100.f;
	// Character->GetCharacterMovement()->GravityScale = 0.f;
	// Character->GetCharacterMovement()->AirControl = 1.f;

	if(Character->GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
}

void UDWCharacterState_Fly::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->ModifyStamina(ATTRIBUTE_DELTAVALUE_CLAMP(Character, Stamina, -Character->GetStaminaExpendSpeed() * 2.5f * DeltaSeconds));

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

void UDWCharacterState_Fly::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Fly);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Flying);

	Character->FreeToAnim();
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	// Character->GetCharacterMovement()->GravityScale = DefaultGravityScale;
	// Character->GetCharacterMovement()->AirControl = DefaultAirControl;
}

void UDWCharacterState_Fly::OnTermination()
{
	Super::OnTermination();
}
