// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Attack.h"

#include "TimerManager.h"
#include "Ability/Components/AbilitySystemComponentBase.h"
#include "Ability/Item/Skill/AbilitySkillBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Character/DWCharacter.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "Character/DWCharacterData.h"

UDWCharacterState_Attack::UDWCharacterState_Attack()
{
	StateName = FName("Attack");
}

void UDWCharacterState_Attack::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWCharacterState_Attack::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	if(!Super::OnEnterValidate(InLastFiniteState)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->DoAction(EDWCharacterActionType::Attack);
}

void UDWCharacterState_Attack::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().AttackingTag);

	Character->LimitToAnim(true, true);
	Character->SetMotionRate(0, 0);
}

void UDWCharacterState_Attack::OnRefresh()
{
	Super::OnRefresh();
}

void UDWCharacterState_Attack::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Attack);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Character->GetCharacterData<UDWCharacterData>().AttackingTag);

	AttackEnd();

	Character->FreeToAnim();
	Character->SetMotionRate(1, 1);
	Character->SetAttackDamageAble(false);
	Character->StopAnimMontage();
	Character->AttackAbilityIndex = 0;
	Character->SkillAbilityID = FPrimaryAssetId();
	Character->AttackType = EDWCharacterAttackType::None;
}

void UDWCharacterState_Attack::OnTermination()
{
	Super::OnTermination();
}

void UDWCharacterState_Attack::AttackStart()
{
	if (!IsCurrentState()) return;
	
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->SetAttackDamageAble(true);
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto SkillAbilityData = Character->GetSkillAbility(Character->SkillAbilityID);
			if(const auto SkillClass = SkillAbilityData.GetItemData<UAbilitySkillDataBase>().SkillClass)
			{
				if(AAbilitySkillBase* Skill = UObjectPoolModuleBPLibrary::SpawnObject<AAbilitySkillBase>(nullptr, SkillClass))
				{
					Skill->Initialize(Character, FAbilityItem(Character->SkillAbilityID));
				}
			}
			break;
		}
		default: break;
	}
}

void UDWCharacterState_Attack::AttackHurt()
{
	if (!IsCurrentState()) return;
	
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->SetAttackDamageAble(false);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([Character](){ Character->SetAttackDamageAble(true); });
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}

void UDWCharacterState_Attack::AttackEnd()
{
	if (!IsCurrentState()) return;
	
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			Character->SetAttackDamageAble(false);
			if (++Character->AttackAbilityIndex >= Character->GetAttackAbilities().Num())
			{
				Character->AttackAbilityIndex = 0;
			}
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			Character->SkillAbilityID = FPrimaryAssetId();
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->StopAnimMontage();
			break;
		}
		default: break;
	}
	Character->AttackType = EDWCharacterAttackType::None;
}
