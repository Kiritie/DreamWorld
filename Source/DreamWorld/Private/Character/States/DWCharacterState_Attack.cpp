// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/States/DWCharacterState_Attack.h"

#include "TimerManager.h"
#include "Ability/AbilityModuleStatics.h"
#include "Ability/Item/Skill/AbilitySkillBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Character/DWCharacter.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Character/DWCharacterData.h"
#include "GameFramework/CharacterMovementComponent.h"

UDWCharacterState_Attack::UDWCharacterState_Attack()
{
	StateName = FName("Attack");
}

void UDWCharacterState_Attack::OnInitialize(UFSMComponent* InFSM, int32 InStateIndex)
{
	Super::OnInitialize(InFSM, InStateIndex);
}

bool UDWCharacterState_Attack::OnEnterValidate(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	if(!Super::OnEnterValidate(InLastState, InParams)) return false;

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	return Character->ControlMode == EDWCharacterControlMode::Fighting && Character->DoAction(EDWCharacterActionType::Attack);
}

void UDWCharacterState_Attack::OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams)
{
	Super::OnEnter(InLastState, InParams);

	if(InParams.IsValidIndex(0))
	{
		OnAttackStart = *InParams[0].GetPointerValue<FSimpleDelegate>();
	}
	if(InParams.IsValidIndex(1))
	{
		OnAttackEnd = *InParams[1].GetPointerValue<FSimpleDelegate>();
	}

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->GetAbilitySystemComponent()->AddLooseGameplayTag(GameplayTags::StateTag_Character_Attacking);
}

void UDWCharacterState_Attack::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void UDWCharacterState_Attack::OnLeave(UFiniteStateBase* InNextState)
{
	Super::OnLeave(InNextState);

	ADWCharacter* Character = GetAgent<ADWCharacter>();

	Character->StopAction(EDWCharacterActionType::Attack);

	Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(GameplayTags::StateTag_Character_Attacking);

	AttackEnd();

	OnAttackStart = nullptr;
	OnAttackEnd = nullptr;
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
			Character->ClearAttackHitTargets();
			Character->SetAttackHitAble(true);
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->ClearAttackHitTargets();
			Character->SetAttackHitAble(true);
			Character->GetCharacterMovement()->GravityScale = Character->DefaultGravityScale * 1.5f;
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto SkillAbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(SkillAbilityData.GetItemData<UAbilitySkillDataBase>().SkillClass)
			{
				UAbilityModuleStatics::SpawnAbilityItem(Character->SkillAbilityItem, Character);
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
	}
}

void UDWCharacterState_Attack::AttackStep()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->AttackType == EDWCharacterAttackType::None) return;
	
	bool bHitAble = !Character->IsAttackHitAble();
	if(bHitAble) Character->ClearAttackHitTargets();
	Character->SetAttackHitAble(bHitAble);
}

void UDWCharacterState_Attack::AttackEnd()
{
	ADWCharacter* Character = GetAgent<ADWCharacter>();

	if (Character->AttackType == EDWCharacterAttackType::None) return;

	switch (Character->AttackType)
	{
		case EDWCharacterAttackType::NormalAttack:
		{
			Character->SetAttackHitAble(false);
			if (++Character->AttackAbilityIndex == Character->GetAttackAbilities().Num())
			{
				Character->AttackAbilityIndex = 0;
			}
			break;
		}
		case EDWCharacterAttackType::FallingAttack:
		{
			Character->SetAttackHitAble(false);
			Character->StopAnimMontage();
			Character->FreeToAnim();
			Character->GetCharacterMovement()->GravityScale = Character->GetDefaultGravityScale();
			break;
		}
		case EDWCharacterAttackType::SkillAttack:
		{
			const auto AbilityData = Character->GetSkillAbility(Character->SkillAbilityItem.ID);
			if(!AbilityData.GetItemData<UAbilitySkillDataBase>().SkillClass)
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
	}
}
