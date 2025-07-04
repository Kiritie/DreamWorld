// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/DWCharacterAnim.h"
#include "Character/DWCharacter.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "Common/CommonStatics.h"

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

void UDWCharacterAnim::NativeHandleNotify(const FString& AnimNotifyName)
{
	Super::NativeHandleNotify(AnimNotifyName);

	ADWCharacter* Character = GetOwnerCharacter<ADWCharacter>();

	if(!Character) return;

	if(AnimNotifyName.Equals(TEXT("Attack start")))
	{
		Character->GetFiniteStateByClass<UDWCharacterState_Attack>()->AttackStart();
	}
	else if(AnimNotifyName.Equals(TEXT("Attack step")))
	{
		Character->GetFiniteStateByClass<UDWCharacterState_Attack>()->AttackStep();
	}
	else if(AnimNotifyName.Equals(TEXT("Attack end")))
	{
		Character->GetFiniteStateByClass<UDWCharacterState_Attack>()->AttackEnd();
	}
	else if(AnimNotifyName.Equals(TEXT("Attack complete")))
	{
		Character->GetFiniteStateByClass<UDWCharacterState_Attack>()->AttackComplete();
	}
}

void UDWCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ADWCharacter* Character = GetOwnerCharacter<ADWCharacter>();

	if(!Character || !Character->Execute_GetAssetID(Character).IsValid() || !UCommonStatics::IsPlaying()) return;

	bSprinting = Character->IsSprinting();
	bAttacking = Character->IsAttacking();
	bDefending = Character->IsDefending();
	bFlying = Character->IsFlying();
	bRiding = Character->IsRiding();
	bClimbing = Character->IsClimbing();
	bCrouching = Character->IsCrouching();
	bSwimming = Character->IsSwimming() || Character->IsFloating();
	bAiming = Character->IsAiming();
}
