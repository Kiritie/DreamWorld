// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DWPlayerCharacterAnim.h"

#include "Character/DWCharacter.h"
#include "Common/CommonStatics.h"
#include "Kismet/KismetMathLibrary.h"

UDWPlayerCharacterAnim::UDWPlayerCharacterAnim()
{
	AimLookUpAngle = 0.f;
}

void UDWPlayerCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ADWCharacter* Character = GetOwnerCharacter<ADWCharacter>();

	if(!Character || !Character->Execute_GetAssetID(Character).IsValid() || !UCommonStatics::IsPlaying()) return;

	if(Character->IsCurrent())
	{
		bAiming = true;
		const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetControlRotation(), Character->GetActorRotation());
		AimLookUpAngle = DeltaRotator.Pitch;
	}
	else
	{
		bAiming = false;
		AimLookUpAngle = 0.f;
	}
}
