// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Attack.h"

#include "TimerManager.h"
#include "Ability/AbilityModuleStatics.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/Projectile/AbilityProjectileBase.h"
#include "Camera/CameraModuleStatics.h"
#include "Character/DWCharacter.h"
#include "Character/States/DWCharacterState_Aim.h"
#include "Common/Looking/LookingComponent.h"
#include "FSM/Components/FSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Equip/Weapon/DWEquipWeaponRemote.h"

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

	return Character->GetAbilitySystemComponent()->TryActivateAbility(InParams[0].GetPointerValueRef<FGameplayAbilitySpecHandle>()) && Character->DoAction(GameplayTags::Ability_Character_Action_Attack);
}

void UDWCharacterState_Attack::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::State_Character_Attacking);

	Character->LimitToAnim();

	Character->AttackType = (EDWCharacterAttackType)InParams[1].GetByteValue();
	Character->AttackWeaponPart = (EDWWeaponPart)InParams[2].GetByteValue();

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			EDWWeaponType WeaponType = Character->GetWeaponType(Character->AttackWeaponPart);
			Character->GetAttackAbilityQueue(WeaponType).Index = InParams[3];
			OnAttackCompleted = InParams[4].GetPointerValueRef<FSimpleDelegate>();
			if(Character->GetWeaponProjectile(Character->AttackWeaponPart))
			{
				Character->SetUseControllerRotation(true);

				const auto AttackAbilityData = Character->GetAttackAbility(WeaponType);
				Character->AttackProjectile = UAbilityModuleStatics::SpawnAbilityProjectile(Character->GetWeaponProjectile(Character->AttackWeaponPart), Character, AttackAbilityData.AbilityHandle);
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			OnAttackCompleted = InParams[3].GetPointerValueRef<FSimpleDelegate>();
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			Character->SkillAttackAbilityItem = InParams[3].GetPointerValueRef<FAbilityItem>();
			OnAttackCompleted = InParams[4].GetPointerValueRef<FSimpleDelegate>();
			if(Character->SkillAttackAbilityItem.GetData<UAbilitySkillDataBase>().ProjectileClass)
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
		FSM->GetStateByClass<UDWCharacterState_Aim>()->EndAim();
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
			if(Character->GetWeaponProjectile(Character->AttackWeaponPart))
			{
				Character->AttackProjectile->Launch(Character->GetLooking()->GetLookingTarget() ? Character->GetLooking()->GetLookingRotation().Vector() :
					(Character->IsPlayer() ? UCameraModuleStatics::GetCameraRotation(true).Vector() : FVector::ZeroVector));
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
			if(Character->SkillAttackAbilityItem.GetData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->AttackProjectile = UAbilityModuleStatics::SpawnAbilityProjectile(Character->SkillAttackAbilityItem.GetData<UAbilitySkillDataBase>().ProjectileClass, Character, Character->SkillAttackAbilityItem.Handle);
				Character->AttackProjectile->Launch(Character->GetLooking()->GetLookingTarget() ? Character->GetLooking()->GetLookingRotation().Vector() :
					(Character->IsPlayer() ? UCameraModuleStatics::GetCameraRotation(true).Vector() : FVector::ZeroVector));
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
			if(!Character->GetWeapon<ADWEquipWeaponRemote>(Character->AttackWeaponPart))
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
			if(!Character->SkillAttackAbilityItem.GetData<UAbilitySkillDataBase>().ProjectileClass)
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
			if(Character->GetWeaponProjectile(Character->AttackWeaponPart))
			{
				Character->SetUseControllerRotation(false);
				if(!Character->AttackProjectile->IsLaunched())
				{
					Character->AttackProjectile->Destroy();
				}
				Character->AttackProjectile = nullptr;
			}
			Character->GetAttackAbilityQueue(Character->GetWeaponType(Character->AttackWeaponPart)).Next();
			Character->AttackWeaponPart = EDWWeaponPart::None;
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->StopAnimMontage();
			Character->GetCharacterMovement()->GravityScale = Character->GetDefaultGravityScale();
			Character->AttackWeaponPart = EDWWeaponPart::None;
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			if(Character->SkillAttackAbilityItem.GetData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->SetUseControllerRotation(false);
				if(!Character->AttackProjectile->IsLaunched())
				{
					Character->AttackProjectile->Destroy();
				}
				Character->AttackProjectile = nullptr;
			}
			Character->SkillAttackAbilityItem = FAbilityItem();
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
