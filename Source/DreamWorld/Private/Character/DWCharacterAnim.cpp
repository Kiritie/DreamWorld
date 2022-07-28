// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterAnim.h"
#include "Character/DWCharacter.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Character/States/DWCharacterState_Death.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "Global/GlobalBPLibrary.h"

UDWCharacterAnim::UDWCharacterAnim()
{
	bFalling = false;
	bSprinting = false;
	bAttacking = false;
	bDefending = false;
	bClimbing = false;
	bCrouching = false;
	bFlying = false;
	bRiding = false;
	bSwimming = false;
	MoveDirection = 0;
	HorizontalSpeed = 0;
	VerticalSpeed = 0;
}

void UDWCharacterAnim::NativeInitializeAnimation()
{
	
}

bool UDWCharacterAnim::HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent)
{
	Super::HandleNotify(AnimNotifyEvent);
	
	ADWCharacter* Character = Cast<ADWCharacter>(TryGetPawnOwner());

	if(!Character) return false;

	const FString NotifyName = AnimNotifyEvent.GetNotifyEventName().ToString().Mid(12);
	if (NotifyName.Equals(TEXT("Free to animate")))
	{
		Character->FreeToAnim();
	}
	else if (NotifyName.Equals(TEXT("Attack start")))
	{
		Character->GetFSMComponent()->GetCurrentState<UDWCharacterState_Attack>()->AttackStart();
	}
	else if (NotifyName.Equals(TEXT("Attack hurt")))
	{
		Character->GetFSMComponent()->GetCurrentState<UDWCharacterState_Attack>()->AttackHurt();
	}
	else if (NotifyName.Equals(TEXT("Attack end")))
	{
		Character->GetFSMComponent()->GetCurrentState<UDWCharacterState_Attack>()->AttackEnd();
	}
	return false;
}

void UDWCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	ADWCharacter* Character = Cast<ADWCharacter>(TryGetPawnOwner());

	if(!Character) return;

	bFalling = Character->IsFalling();

	bSprinting = Character->IsSprinting();
	bAttacking = Character->IsAttacking();
	bDefending = Character->IsDefending();
	bFlying = Character->IsFlying();
	bRiding = Character->IsRiding();
	bClimbing = Character->IsClimbing();
	bCrouching = Character->IsCrouching();
	bSwimming = Character->IsSwimming();

	VerticalSpeed = Character->GetMoveVelocity(false).Z;
	HorizontalSpeed = Character->GetMoveVelocity().Size();

	MoveDirection = FMath::FindDeltaAngleDegrees(Character->GetMoveDirection().ToOrientationRotator().Yaw, Character->GetActorRotation().Yaw);
}
