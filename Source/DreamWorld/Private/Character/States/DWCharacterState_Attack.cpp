// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Attack.h"

#include "TimerManager.h"
#include "Ability/AbilityModuleStatics.h"
#include "Ability/Components/AbilitySystemComponentBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/Projectile/AbilityProjectileBase.h"
#include "Camera/CameraModuleStatics.h"
#include "Character/DWCharacter.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Character/DWCharacterData.h"
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

	return Character->ControlMode == EDWCharacterControlMode::Fighting && Character->GetAbilitySystemComponent()->TryActivateAbility(InParams[0].GetPointerValueRef<FGameplayAbilitySpecHandle>()) && Character->DoAction(GameplayTags::AbilityTag_Character_Action_Attack);
}

void UDWCharacterState_Attack::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Attacking);

	Character->AttackType = (EDWCharacterAttackType)InParams[1].GetByteValue();

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			Character->AttackAbilityIndex = InParams[2];
			OnAttackStart = InParams[3].GetPointerValueRef<FSimpleDelegate>();
			OnAttackEnd = InParams[4].GetPointerValueRef<FSimpleDelegate>();
			if(Character->GetWeapon<ADWEquipWeaponRemote>())
			{
				Character->bUseControllerRotationYaw = true;
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			OnAttackStart = InParams[2].GetPointerValueRef<FSimpleDelegate>();
			OnAttackEnd = InParams[3].GetPointerValueRef<FSimpleDelegate>();
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			Character->SkillAbilityItem = InParams[2].GetPointerValueRef<FAbilityItem>();
			OnAttackStart = InParams[3].GetPointerValueRef<FSimpleDelegate>();
			OnAttackEnd = InParams[4].GetPointerValueRef<FSimpleDelegate>();
			const auto SkillAbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(SkillAbilityData.GetItemData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->bUseControllerRotationYaw = true;
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

	Character->StopAction(GameplayTags::AbilityTag_Character_Action_Attack);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Attacking);

	if(Character->IsAiming())
	{
		Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Aiming);

		Character->bUseControllerRotationYaw = false;

		UCameraModuleStatics::DoCameraOffset(FVector(-1.f), 0.5f, EEaseType::InOutSine);
		UCameraModuleStatics::DoCameraFov(-1.f, 0.5f, EEaseType::InOutSine);
	}

	AttackEnd();
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
			if(ADWEquipWeaponRemote* Weapon = Character->GetWeapon<ADWEquipWeaponRemote>())
			{
				const auto AttackAbilityData = Character->GetAttackAbility(Character->AttackAbilityIndex);
				UAbilityModuleStatics::SpawnAbilityProjectile(Weapon->GetItemData<UDWEquipWeaponRemoteData>().ProjectileClass, Character, AttackAbilityData.AbilityHandle);
			}
			else
			{
				Character->ClearAttackHitTargets();
				Character->SetAttackHitAble(true);
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->ClearAttackHitTargets();
			Character->SetAttackHitAble(true);
			Character->GetCharacterMovement()->GravityScale = Character->GetDefaultGravityScale() * 1.5f;
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto SkillAbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(SkillAbilityData.GetItemData<UAbilitySkillDataBase>().ProjectileClass)
			{
				UAbilityModuleStatics::SpawnAbilityProjectile(SkillAbilityData.GetItemData<UAbilitySkillDataBase>().ProjectileClass, Character, Character->SkillAbilityItem.AbilityHandle);
			}
			else
			{
				Character->ClearAttackHitTargets();
				Character->SetAttackHitAble(true);
			}
			break;
		}
		default: break;
	}

	if(OnAttackStart.IsBound())
	{
		OnAttackStart.Execute();
		OnAttackStart = nullptr;
	}
}

void UDWCharacterState_Attack::AttackStep()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->AttackType == EDWCharacterAttackType::None) return;

	if (Character->AttackType == EDWCharacterAttackType::SkillAttack)
	{
		const auto AbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
		if(AbilityData.GetItemData<UAbilitySkillDataBase>().ProjectileClass) return;
	}
	
	bool bAttackHitAble = !Character->IsAttackHitAble();
	if(bAttackHitAble) Character->ClearAttackHitTargets();
	Character->SetAttackHitAble(bAttackHitAble);
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
				Character->SetAttackHitAble(false);
			}
			if (++Character->AttackAbilityIndex == Character->GetAttackAbilities().Num())
			{
				Character->AttackAbilityIndex = 0;
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->StopAnimMontage();
			Character->SetAttackHitAble(false);
			Character->GetCharacterMovement()->GravityScale = Character->GetDefaultGravityScale();
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto AbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(!AbilityData.GetItemData<UAbilitySkillDataBase>().ProjectileClass)
			{
				Character->SetAttackHitAble(false);
			}
			Character->SkillAbilityItem = FAbilityItem();
			break;
		}
		default: break;
	}

	Character->AttackType = EDWCharacterAttackType::None;

	if(OnAttackEnd.IsBound())
	{
		OnAttackEnd.Execute();
		OnAttackEnd = nullptr;
	}
}
