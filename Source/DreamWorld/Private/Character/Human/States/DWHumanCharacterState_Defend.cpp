// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Human/States/DWHumanCharacterState_Defend.h"

#include "Character/DWCharacter.h"
#include "Item/Equip/Weapon/DWEquipWeapon.h"

UDWHumanCharacterState_Defend::UDWHumanCharacterState_Defend()
{
}

void UDWHumanCharacterState_Defend::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWHumanCharacterState_Defend::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->CheckWeaponType(EDWWeaponPart::Secondary, EDWWeaponType::Shield) && Super::OnPreEnter(InLastState, InParams);
}

void UDWHumanCharacterState_Defend::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetWeapon(EDWWeaponPart::Secondary)->Active();
}

void UDWHumanCharacterState_Defend::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWHumanCharacterState_Defend::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetWeapon(EDWWeaponPart::Secondary)->Deactive();
}

void UDWHumanCharacterState_Defend::OnTermination()
{
	Super::OnTermination();
}
