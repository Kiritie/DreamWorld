// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Aim.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/Projectile/AbilityProjectileBase.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/States/DWCharacterState_Attack.h"
#include "FSM/Components/FSMComponent.h"
#include "Item/Equip/Weapon/DWEquipWeaponRemote.h"
#include "Item/Equip/Weapon/DWEquipWeaponRemoteData.h"

UDWCharacterState_Aim::UDWCharacterState_Aim()
{
	StateName = FName("Aim");
}

void UDWCharacterState_Aim::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Aim::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(GameplayTags::Ability_Character_Action_Aim);
}

void UDWCharacterState_Aim::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();
	
	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Aiming);

	Character->SetMotionRate(0.5f, 0.1f);

	if(ADWEquipWeaponRemote* Weapon = Character->GetWeapon<ADWEquipWeaponRemote>())
	{
		const auto AttackAbilityData = Character->GetAttackAbility();
		Character->AttackProjectile = UAbilityModuleStatics::SpawnAbilityProjectile(Weapon->GetItemData<UDWEquipWeaponRemoteData>().ProjectileClass, Character, AttackAbilityData.AbilityHandle);
	}
}

void UDWCharacterState_Aim::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Aim::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::Ability_Character_Action_Aim);

	Character->SetMotionRate(1.f, 1.f);

	Character->AttackProjectile->Destroy();

	if(!InNextState || !InNextState->IsA<UDWCharacterState_Attack>())
	{
		Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Aiming);
	}
}

void UDWCharacterState_Aim::OnTermination()
{
	Super::OnTermination();
}
