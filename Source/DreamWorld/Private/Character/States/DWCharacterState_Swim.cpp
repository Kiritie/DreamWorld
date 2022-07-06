// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Swim.h"

#include "Character/DWCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Scene/SceneModuleBPLibrary.h"
#include "Scene/Actor/PhysicsVolume/PhysicsVolumeBase.h"

UDWCharacterState_Swim::UDWCharacterState_Swim()
{
	StateName = FName("Swim");
}

void UDWCharacterState_Swim::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Swim::OnValidate()
{
	if(!Super::OnValidate()) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Swim);
}

void UDWCharacterState_Swim::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
	if(USceneModuleBPLibrary::HasPhysicsVolumeByName(FName("Water")))
	{
		if(Character->GetCharacterMovement()->UpdatedComponent)
		{
			Character->GetCharacterMovement()->UpdatedComponent->SetPhysicsVolume(USceneModuleBPLibrary::GetPhysicsVolumeByName(FName("Water")), true);
		}
	}
	//FVector Velocity = Character->GetMovementComponent()->Velocity;
	//Character->GetMovementComponent()->Velocity = FVector(Velocity.X, Velocity.Y, 0);
}

void UDWCharacterState_Swim::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Swim::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Swim);

	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if(Character->GetCharacterMovement()->UpdatedComponent)
	{
		Character->GetCharacterMovement()->UpdatedComponent->SetPhysicsVolume(GetWorld()->GetDefaultPhysicsVolume(), true);
	}
	//FVector Velocity = Character->GetMovementComponent()->Velocity;
	//Character->GetMovementComponent()->Velocity = FVector(Velocity.X, Velocity.Y, 0);
}

void UDWCharacterState_Swim::OnTermination()
{
	Super::OnTermination();
}
