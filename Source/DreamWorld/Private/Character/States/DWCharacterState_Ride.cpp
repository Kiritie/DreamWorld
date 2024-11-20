// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Ride.h"

#include "Character/CharacterModuleStatics.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Scene/SceneModuleStatics.h"
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

bool UDWCharacterState_Ride::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::Ability_Character_Action_Ride);
}

void UDWCharacterState_Ride::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	ADWCharacter* RidingTarget = nullptr;

	if(InParams.IsValidIndex(0))
	{
		RidingTarget = InParams[0];
	}

	Character->RidingTarget = RidingTarget;

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Riding);

	UCharacterModuleStatics::SwitchCharacter(RidingTarget);
	
	Character->GetMovementComponent()->SetActive(false);
	Character->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RidingTarget->AttachActor(Character, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RiderPoint"));
	Character->SetInteractingAgent(RidingTarget, true);
	// Character->LimitToAnim();
	
	RidingTarget->SetMotionRate(1.f, 1.f);
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

	Character->StopAction(GameplayTags::Ability_Character_Action_Ride);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Riding);

	RidingTarget->DetachActor(Character, FDetachmentTransformRules::KeepWorldTransform);

	UCharacterModuleStatics::SwitchCharacter(Character);

	if(Character->IsActive())
	{
		// Character->FreeToAnim();
		Character->GetMovementComponent()->SetActive(true);
		Character->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	if(RidingTarget)
	{
		const FVector Offset = Character->GetActorRightVector() * (Character->GetRadius() + RidingTarget->GetRadius());
		FVector RayStart = Character->GetActorLocation() + Offset;
		const FVector RayEnd = FVector(RayStart.X, RayStart.Y, 0);

		FHitResult HitResult;
		if(UVoxelModule::IsValid())
		{
			RayStart.Z = UVoxelModuleStatics::GetWorldData().GetWorldRealSize().Z;
			UVoxelModuleStatics::VoxelAgentTraceSingle(RayStart, RayEnd, Character->GetRadius(), Character->GetHalfHeight(), {}, HitResult, false);
		}
		else
		{
			RayStart.Z = 10000.f;
			UKismetSystemLibrary::CapsuleTraceSingle(GetWorldContext(), RayStart, RayEnd, Character->GetRadius(), Character->GetHalfHeight(), USceneModuleStatics::GetTraceMapping(FName("Chunk")).GetTraceType(), false, {}, EDrawDebugTrace::None, HitResult, true);
		}
		if (HitResult.bBlockingHit)
		{
			Character->SetActorLocation(HitResult.Location);
		}
	}
	
	Character->RidingTarget = nullptr;
}

void UDWCharacterState_Ride::OnTermination()
{
	Super::OnTermination();
}
