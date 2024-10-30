// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Attack.h"

#include "TimerManager.h"
#include "Ability/AbilityModuleStatics.h"
#include "Ability/Components/AbilitySystemComponentBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/Projectile/AbilityProjectileBase.h"
#include "Camera/CameraModuleStatics.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Common/Looking/LookingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Equip/Weapon/DWEquipWeaponRemote.h"
#include "Item/Equip/Weapon/DWEquipWeaponRemoteData.h"

UDWCharacterState_Attack::UDWCharacterState_Attack()
{
	StateName = FName("Attack");
}

void UDWCharacterState_Attack::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Attack::OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnPreEnter(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->ControlMode == EDWCharacterControlMode::Fighting && Character->GetAbilitySystemComponent()->TryActivateAbility(InParams[0].GetPointerValueRef<FGameplayAbilitySpecHandle>()) && Character->DoAction(GameplayTags::Ability_Character_Action_Attack);
}

void UDWCharacterState_Attack::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Attacking);

	Character->LimitToAnim();

	Character->AttackType = (EDWCharacterAttackType)InParams[1].GetByteValue();

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			Character->GetAttackAbilityQueue().AbilityIndex = InParams[2];
			OnAttackCompleted = InParams[3].GetPointerValueRef<FSimpleDelegate>();
			if(ADWEquipWeaponRemote* Weapon = Character->GetWeapon<ADWEquipWeaponRemote>())
			{
				Character->SetUseControllerRotation(true);

				const auto AttackAbilityData = Character->GetAttackAbility();
				Character->AttackProjectile = UAbilityModuleStatics::SpawnAbilityProjectile(Weapon->GetItemData<UDWEquipWeaponRemoteData>().ProjectileClass, Character, AttackAbilityData.AbilityHandle);
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			OnAttackCompleted = InParams[2].GetPointerValueRef<FSimpleDelegate>();
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			Character->SkillAbilityItem = InParams[2].GetPointerValueRef<FAbilityItem>();
			OnAttackCompleted = InParams[3].GetPointerValueRef<FSimpleDelegate>();
			const auto SkillAbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(SkillAbilityData.GetData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->SetUseControllerRotation(true);
			}
			break;
		}
		default: break;
	}
}

void UDWCharacterState_Attack::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Attack::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(GameplayTags::Ability_Character_Action_Attack);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Attacking);

	Character->FreeToAnim();

	if(Character->IsAiming())
	{
		Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::State_Character_Aiming);

		Character->SetUseControllerRotation(false);

		UCameraModuleStatics::DoCameraOffset(FVector(-1.f), 0.5f, EEaseType::InOutSine);
		UCameraModuleStatics::DoCameraFov(-1.f, 0.5f, EEaseType::InOutSine);
	}

	AttackEnd();
	AttackComplete();
}

void UDWCharacterState_Attack::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Attack::AttackStart()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->AttackType == EDWCharacterAttackType::None) return;

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			if(Character->GetWeapon<ADWEquipWeaponRemote>())
			{
				Character->AttackProjectile->Launch(Character->GetLooking()->GetLookingTarget() ? Character->GetLooking()->GetLookingRotation().Vector() : (Character->IsPlayer() ? UCameraModuleStatics::GetCameraRotation(true).Vector() : FVector::ZeroVector));
			}
			else
			{
				Character->ClearHitTargets();
				Character->SetHitAble(true);
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->ClearHitTargets();
			Character->SetHitAble(true);
			Character->GetCharacterMovement()->GravityScale = Character->GetDefaultGravityScale() * 1.5f;
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto SkillAbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(SkillAbilityData.GetData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->AttackProjectile = UAbilityModuleStatics::SpawnAbilityProjectile(SkillAbilityData.GetData<UAbilitySkillDataBase>().ProjectileClass, Character, Character->SkillAbilityItem.AbilityHandle);
				Character->AttackProjectile->Launch(Character->GetLooking()->GetLookingTarget() ? Character->GetLooking()->GetLookingRotation().Vector() : (Character->IsPlayer() ? UCameraModuleStatics::GetCameraRotation(true).Vector() : FVector::ZeroVector));
			}
			else
			{
				Character->ClearHitTargets();
				Character->SetHitAble(true);
			}
			break;
		}
		default: break;
	}
}

void UDWCharacterState_Attack::AttackStep()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->AttackType == EDWCharacterAttackType::None) return;

	if (Character->AttackType == EDWCharacterAttackType::SkillAttack)
	{
		const auto AbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
		if(AbilityData.GetData<UAbilitySkillDataBase>().ProjectileClass) return;
	}
	
	bool bHitAble = !Character->IsHitAble();
	if(bHitAble) Character->ClearHitTargets();
	Character->SetHitAble(bHitAble);
}

void UDWCharacterState_Attack::AttackEnd()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->AttackType == EDWCharacterAttackType::None) return;

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			if(!Character->GetWeapon<ADWEquipWeaponRemote>())
			{
				Character->SetHitAble(false);
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->SetHitAble(false);
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto AbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(!AbilityData.GetData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->SetHitAble(false);
			}
			break;
		}
		default: break;
	}
}

void UDWCharacterState_Attack::AttackComplete()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->AttackType == EDWCharacterAttackType::None) return;

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			if(Character->GetWeapon<ADWEquipWeaponRemote>())
			{
				Character->SetUseControllerRotation(false);
				Character->AttackProjectile = nullptr;
			}
			Character->GetAttackAbilityQueue().Next();
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->StopAnimMontage();
			Character->GetCharacterMovement()->GravityScale = Character->GetDefaultGravityScale();
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto AbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(AbilityData.GetData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->SetUseControllerRotation(false);
				Character->AttackProjectile = nullptr;
			}
			Character->SkillAbilityItem = FAbilityItem();
			break;
		}
		default: break;
	}

	Character->AttackType = EDWCharacterAttackType::None;

	if(OnAttackCompleted.IsBound())
	{
		OnAttackCompleted.Execute();
		OnAttackCompleted = nullptr;
	}
}
