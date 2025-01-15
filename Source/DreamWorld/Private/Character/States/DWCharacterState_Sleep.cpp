// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Sleep.h"

#include "AI/Base/AIControllerBase.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Common/Interaction/InteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Scene/SceneModuleStatics.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"

UDWCharacterState_Sleep::UDWCharacterState_Sleep()
{
	StateName = FName("Sleep");
}

void UDWCharacterState_Sleep::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Sleep::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::Ability_Character_Action_Sleep);
}

void UDWCharacterState_Sleep::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if(InParams.IsValidIndex(0))
	{
		Bed = InParams[0];
	}

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Vitality_Active);
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Sleeping);

	Character->LimitToAnim();

	if(Character->GetController<AAIControllerBase>())
	{
		Character->GetController<AAIControllerBase>()->StopBehaviorTree();
	}

	Character->GetMovementComponent()->SetActive(false);
	Character->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Character->SetInteractingAgent(Bed, true);
	Character->SetActorLocationAndRotation(Bed->GetActorLocation() + FVector::UpVector * (Character->GetHalfHeight()/* + UVoxelModule::Get().GetWorldData().BlockSize * 0.25f*/), Bed->GetActorRotation());
}

void UDWCharacterState_Sleep::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Sleep::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::Ability_Character_Action_Sleep);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Sleeping);
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Vitality_Active);

	Character->FreeToAnim();

	if(Character->GetController<AAIControllerBase>())
	{
		Character->GetController<AAIControllerBase>()->RunBehaviorTree();
	}

	Character->GetMovementComponent()->SetActive(true);
	Character->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	Character->SetInteractingAgent(nullptr, true);
	
	Bed = nullptr;
}

void UDWCharacterState_Sleep::OnTermination()
{
	Super::OnTermination();
}
