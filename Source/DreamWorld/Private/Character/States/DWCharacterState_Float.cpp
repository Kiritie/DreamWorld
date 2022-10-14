// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Float.h"

#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/DWCharacterPart.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/States/DWCharacterState_Swim.h"
#include <Scene/SceneModuleBPLibrary.h>
#include "Scene/Actor/PhysicsVolume/PhysicsVolumeBase.h"

UDWCharacterState_Float::UDWCharacterState_Float()
{
	StateName = FName("Float");

	WaterPosZ = 0.f;
}

void UDWCharacterState_Float::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Float::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Float);
}

void UDWCharacterState_Float::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().FloatingTag);

	Character->LimitToAnim();

	if(!InLastFiniteState || !InLastFiniteState->IsA<UDWCharacterState_Swim>())
	{
		if(USceneModuleBPLibrary::HasPhysicsVolumeByName(FName("Water")))
		{
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
			if(Character->GetCharacterMovement()->UpdatedComponent)
			{
				Character->GetCharacterMovement()->UpdatedComponent->SetPhysicsVolume(USceneModuleBPLibrary::GetPhysicsVolumeByName(FName("Water")), true);
			}
		}
	}
	Character->GetCharacterMovement()->Velocity = FVector(Character->GetCharacterMovement()->Velocity.X, Character->GetCharacterMovement()->Velocity.Y, 0);
	const float NeckPosZ = Character->GetCharacterPart(EDWCharacterPartType::Neck)->GetComponentLocation().Z;
	const float ChestPosZ = Character->GetCharacterPart(EDWCharacterPartType::Chest)->GetComponentLocation().Z;
	const float OffsetZ = ChestPosZ + (NeckPosZ - ChestPosZ) * 0.35f - Character->GetActorLocation().Z;
	Character->SetActorLocation(FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, WaterPosZ - OffsetZ));
}

void UDWCharacterState_Float::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Float::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Float);
	
	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().FloatingTag);

	Character->FreeToAnim();

	if(!InNextFiniteState || !InNextFiniteState->IsA<UDWCharacterState_Swim>())
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		if(Character->GetCharacterMovement()->UpdatedComponent)
		{
			Character->GetCharacterMovement()->UpdatedComponent->SetPhysicsVolume(USceneModuleBPLibrary::GetDefaultPhysicsVolume(), true);
		}
		Character->GetCharacterMovement()->Velocity = FVector(Character->GetCharacterMovement()->Velocity.X, Character->GetCharacterMovement()->Velocity.Y, Character->GetCharacterMovement()->Velocity.Z * 0.5f);
	}
}

void UDWCharacterState_Float::OnTermination()
{
	Super::OnTermination();
}
