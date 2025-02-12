#include "Ability/Attributes/DWCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Character/DWCharacter.h"
#include "Common/DWCommonTypes.h"
#include "ReferencePool/ReferencePoolModuleStatics.h"

UDWCharacterAttributeSet::UDWCharacterAttributeSet()
	: Mana(100.f)
	, MaxMana(100.f)
	, ManaRecovery(0.f)
	, ManaRegenSpeed(1.f)
	, Stamina(100.f)
	, MaxStamina(100.f)
	, StaminaRecovery(0.f)
	, StaminaRegenSpeed(10.f)
	, StaminaExpendSpeed(5.f)
	, Hunger(100.f)
	, HungerRecovery(0.f)
	, HungerExpendSpeed(0.1f)
	, Thirst(100.f)
	, ThirstRecovery(0.f)
	, ThirstExpendSpeed(0.15f)
	, Oxygen(100.f)
	, OxygenRecovery(0.f)
	, OxygenRegenSpeed(10.f)
	, OxygenExpendSpeed(3.f)
	, DodgeForce(500.f)
	, RepulseForce(0.f)
	, AttackForce(0.f)
	, AttackSpeed(1.f)
	, AttackCritRate(0.f)
	, AttackStealRate(0.f)
	, DefendRate(0.f)
	, DefendScopeRate(0.f)
	, PhysicsRes(0.f)
	, MagicRes(0.f)
	, ToughnessRate(0.f)
{
	DamageHandleClass = UDWDamageHandle::StaticClass();
}

void UDWCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetManaRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetStaminaRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetStaminaExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetHungerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	else if (Attribute == GetHungerRecoveryAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetHungerExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetThirstAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	else if (Attribute == GetThirstRecoveryAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetThirstExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetOxygenAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	else if (Attribute == GetOxygenRecoveryAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetOxygenRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetOxygenExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetDodgeForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetAttackForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetRepulseForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetAttackCritRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetAttackStealRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetDefendRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetDefendScopeRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetPhysicsResAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetMagicResAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetToughnessRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
}

void UDWCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetManaRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetStaminaRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetStaminaExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetHungerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	else if (Attribute == GetHungerRecoveryAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetHungerExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetThirstAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	else if (Attribute == GetThirstRecoveryAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetThirstExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetOxygenAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	else if (Attribute == GetOxygenRecoveryAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetOxygenRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetOxygenExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetDodgeForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetAttackForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetRepulseForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetAttackCritRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetAttackStealRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetDefendRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetDefendScopeRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetPhysicsResAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetMagicResAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetToughnessRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
}

void UDWCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(GetManaAttribute(), OldValue, NewValue);
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(GetStaminaAttribute(), OldValue, NewValue);
	}
}

void UDWCharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	AActor* SourceActor = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
	}
	FHitResult HitResult;
	if (Context.GetHitResult())
	{
		HitResult = *Context.GetHitResult();
	}

	AActor* TargetActor = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	}

	if(Data.EvaluatedData.Attribute == GetManaRecoveryAttribute())
	{
		UReferencePoolModuleStatics::GetReference<URecoveryHandle>(true, RecoveryHandleClass).HandleRecovery(SourceActor, TargetActor, GetManaRecovery(), GetManaRecoveryAttribute(), HitResult, SourceTags);
		SetManaRecovery(0.f);
	}
	else if(Data.EvaluatedData.Attribute == GetStaminaRecoveryAttribute())
	{
		UReferencePoolModuleStatics::GetReference<URecoveryHandle>(true, RecoveryHandleClass).HandleRecovery(SourceActor, TargetActor, GetStaminaRecovery(), GetStaminaRecoveryAttribute(), HitResult, SourceTags);
		SetStaminaRecovery(0.f);
	}
	else if(Data.EvaluatedData.Attribute == GetHungerRecoveryAttribute())
	{
		UReferencePoolModuleStatics::GetReference<URecoveryHandle>(true, RecoveryHandleClass).HandleRecovery(SourceActor, TargetActor, GetHungerRecovery(), GetHungerRecoveryAttribute(), HitResult, SourceTags);
		SetHungerRecovery(0.f);
	}
	else if(Data.EvaluatedData.Attribute == GetThirstRecoveryAttribute())
	{
		UReferencePoolModuleStatics::GetReference<URecoveryHandle>(true, RecoveryHandleClass).HandleRecovery(SourceActor, TargetActor, GetThirstRecovery(), GetThirstRecoveryAttribute(), HitResult, SourceTags);
		SetThirstRecovery(0.f);
	}
	else if(Data.EvaluatedData.Attribute == GetOxygenRecoveryAttribute())
	{
		UReferencePoolModuleStatics::GetReference<URecoveryHandle>(true, RecoveryHandleClass).HandleRecovery(SourceActor, TargetActor, GetOxygenRecovery(), GetOxygenRecoveryAttribute(), HitResult, SourceTags);
		SetOxygenRecovery(0.f);
	}
}
