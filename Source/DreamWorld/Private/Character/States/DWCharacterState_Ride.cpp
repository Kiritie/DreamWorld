// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Ride.h"

#include "Character/CharacterModuleStatics.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Components/CapsuleComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleStatics.h"

UDWCharacterState_Ride::UDWCharacterState_Ride()
{
	StateName = FName("Ride");

}

void UDWCharacterState_Ride::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Ride::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnEnterValidate(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Ride);
}

void UDWCharacterState_Ride::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	ADWCharacter* RidingTarget = Character->GetRidingTarget();

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Riding);

	UCharacterModuleStatics::SwitchCharacter(RidingTarget);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Character->AttachToComponent(RidingTarget->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("RiderPoint"));
	Character->SetActorRelativeLocation(FVector::ZeroVector);
	Character->SetActorRotation(RidingTarget->GetActorRotation());
	Character->LimitToAnim();
}

void UDWCharacterState_Ride::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Ride::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	ADWCharacter* RidingTarget = Character->GetRidingTarget();

	Character->StopAction(EDWCharacterActionType::Ride);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Riding);

	Character->FreeToAnim();
	if(Character->IsActive()) Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if(RidingTarget)
	{
		UCharacterModuleStatics::SwitchCharacter(Character);
		FHitResult hitResult;
		const FVector offset = Character->GetActorRightVector() * (Character->GetRadius() + RidingTarget->GetRadius());
		const FVector rayStart = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, UVoxelModule::Get().GetWorldData().GetWorldRealSize().Z) + offset;
		const FVector rayEnd = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, 0) + offset;
		if (UVoxelModuleStatics::VoxelAgentTraceSingle(rayStart, rayEnd, Character->GetRadius(), Character->GetHalfHeight(), {}, hitResult))
		{
			Character->SetActorLocation(hitResult.Location);
		}
	}
	Character->RidingTarget = nullptr;
}

void UDWCharacterState_Ride::OnTermination()
{
	Super::OnTermination();
}
