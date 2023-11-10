// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DWCharacterAnim.h"
#include "Character/DWCharacter.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "FSM/Components/FSMComponent.h"
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

	ADWCharacter* Character = Cast<ADWCharacter>(TryGetPawnOwner());

	if(!Character) return;

	if(AnimNotifyName.Equals(TEXT("Free to animate")))
	{
		Character->FreeToAnim();
	}
	else if(AnimNotifyName.Equals(TEXT("Limit to animate")))
	{
		Character->LimitToAnim();
	}
	else if(AnimNotifyName.Equals(TEXT("Attack start")))
	{
		Character->GetFSMComponent()->GetStateByClass<UDWCharacterState_Attack>()->AttackStart();
	}
	else if(AnimNotifyName.Equals(TEXT("Attack step")))
	{
		Character->GetFSMComponent()->GetStateByClass<UDWCharacterState_Attack>()->AttackStep();
	}
	else if(AnimNotifyName.Equals(TEXT("Attack end")))
	{
		Character->GetFSMComponent()->GetStateByClass<UDWCharacterState_Attack>()->AttackEnd();
	}
}

void UDWCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ADWCharacter* Character = Cast<ADWCharacter>(TryGetPawnOwner());

	if(!Character || !Character->Execute_GetAssetID(Character).IsValid() || !UCommonStatics::IsPlaying()) return;

	bSprinting = Character->IsSprinting();
	bAttacking = Character->IsAttacking();
	bDefending = Character->IsDefending();
	bFlying = Character->IsFlying();
	bRiding = Character->IsRiding();
	bClimbing = Character->IsClimbing();
	bCrouching = Character->IsCrouching();
	bSwimming = Character->IsSwimming() || Character->IsFloating();
}
