// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Ride.h"

#include "Character/CharacterModuleBPLibrary.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Components/CapsuleComponent.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/VoxelModuleBPLibrary.h"

UDWCharacterState_Ride::UDWCharacterState_Ride()
{
	StateName = FName("Ride");

}

void UDWCharacterState_Ride::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Ride::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Ride);
}

void UDWCharacterState_Ride::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	ADWCharacter* RidingTarget = Character->GetRidingTarget();

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().RidingTag);

	UCharacterModuleBPLibrary::SwitchCharacter(RidingTarget);
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Character->AttachToComponent(RidingTarget->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("RiderPoint"));
	Character->SetActorRelativeLocation(FVector::ZeroVector);
	Character->SetActorRotation(RidingTarget->GetActorRotation());
	Character->LimitToAnim();
}

void UDWCharacterState_Ride::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Ride::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	ADWCharacter* RidingTarget = Character->GetRidingTarget();

	Character->StopAction(EDWCharacterActionType::Ride);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().RidingTag);

	Character->FreeToAnim();
	if(Character->IsActive()) Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if(RidingTarget)
	{
		UCharacterModuleBPLibrary::SwitchCharacter(Character);
		FHitResult hitResult;
		const FVector offset = Character->GetActorRightVector() * (Character->GetRadius() + RidingTarget->GetRadius());
		const FVector rayStart = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, UVoxelModuleBPLibrary::GetWorldData().ChunkHeightRange * UVoxelModuleBPLibrary::GetWorldData().GetChunkLength() + 500) + offset;
		const FVector rayEnd = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, 0) + offset;
		if (UVoxelModuleBPLibrary::ChunkTraceSingle(rayStart, rayEnd, Character->GetRadius(), Character->GetHalfHeight(), hitResult))
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
