// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Swim.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Scene/SceneModuleStatics.h"
#include "Character/States/DWCharacterState_Float.h"

UDWCharacterState_Swim::UDWCharacterState_Swim()
{
	StateName = FName("Swim");
}

void UDWCharacterState_Swim::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Swim::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnEnterValidate(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Swim);
}

void UDWCharacterState_Swim::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Swimming);

	// Character->LimitToAnim();

	if(Character->GetCharacterMovement()->MovementMode != MOVE_Swimming)
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
	}

	Character->GetMovementComponent()->Velocity.Z *= 0.2f;
}

void UDWCharacterState_Swim::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Swim::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Swim);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Swimming);
	
	// Character->FreeToAnim();

	// const FVoxelItem& NeckOverlappingVoxel = Character->GetCharacterPart(EDWCharacterPartType::Chest)->GetOverlappingVoxel();
	//
	// if(!NeckOverlappingVoxel.IsValid() || NeckOverlappingVoxel.GetVoxelType() != EVoxelType::Water)
	// {
	// 	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	// }
}

void UDWCharacterState_Swim::OnTermination()
{
	Super::OnTermination();
}
