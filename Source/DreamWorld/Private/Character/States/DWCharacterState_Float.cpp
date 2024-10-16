// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Float.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/DWCharacterPart.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/States/DWCharacterState_Swim.h"
#include "FSM/Components/FSMComponent.h"

UDWCharacterState_Float::UDWCharacterState_Float()
{
	StateName = FName("Float");

	WaterPosZ = -1.f;
}

void UDWCharacterState_Float::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Float::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::Ability_Character_Action_Float);
}

void UDWCharacterState_Float::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	if(InParams.IsValidIndex(0))
	{
		WaterPosZ = InParams[0];
	}

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Floating);

	// Character->LimitToAnim();

	if(Character->GetCharacterMovement()->MovementMode != MOVE_Swimming)
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
	}

	Character->GetCharacterMovement()->Velocity.Z = 0;
}

void UDWCharacterState_Float::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(WaterPosZ != -1.f)
	{
		const float NeckPosZ = Character->GetCharacterPart(EDWCharacterPartType::Neck)->GetComponentLocation().Z;
		const float ChestPosZ = Character->GetCharacterPart(EDWCharacterPartType::Chest)->GetComponentLocation().Z;
		const float OffsetZ = ChestPosZ + (NeckPosZ - ChestPosZ) * 0.5f - Character->GetActorLocation().Z;
		const float TargetPosZ = WaterPosZ - OffsetZ;
		Character->SetActorLocation(FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, FMath::FInterpConstantTo(Character->GetActorLocation().Z, TargetPosZ, DeltaSeconds, Character->GetSwimSpeed())));
		if(FMath::IsNearlyEqual(Character->GetActorLocation().Z, TargetPosZ, 0.2f))
		{
			WaterPosZ = -1.f;
		}
	}
}

void UDWCharacterState_Float::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::Ability_Character_Action_Float);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Floating);

	// Character->FreeToAnim();

	// const FVoxelItem& ChestOverlappingVoxel = Character->GetCharacterPart(EDWCharacterPartType::Chest)->GetOverlappingVoxel();
	//
	// if(!ChestOverlappingVoxel.IsValid() || ChestOverlappingVoxel.GetVoxelType() != EVoxelType::Water || (InNextState && InNextState->IsA<UDWCharacterState_Jump>()))
	// {
	// 	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	// }
}

void UDWCharacterState_Float::OnTermination()
{
	Super::OnTermination();
}
