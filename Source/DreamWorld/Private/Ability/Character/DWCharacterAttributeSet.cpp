#include "Ability/Character/DWCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Character/DWCharacter.h"
#include "Global/DWGlobalTypes.h"

UDWCharacterAttributeSet::UDWCharacterAttributeSet()
:	Mana(100.f),
	MaxMana(100.f),
	Stamina(100.f),
	MaxStamina(100.f),
	SwimSpeed(350.f),
	RideSpeed(350.f),
	FlySpeed(350.f),
	DodgeForce(500.f),
	AttackForce(10.f),
	RepulseForce(0.f),
	AttackSpeed(1.f),
	AttackCritRate(0.f),
	AttackStealRate(0.f),
	DefendRate(0.f),
	DefendScope(0.f),
	PhysicsDefRate(0.f),
	MagicDefRate(0.f),
	ToughnessRate(0.f),
	StaminaRegenSpeed(10.f),
	StaminaExpendSpeed(5.f)
{
	DamageHandle = UDWDamageHandle::StaticClass();
}

void UDWCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(AbilityComp->GetAvatarActor());
	
	const float CurrentValue = Attribute.GetGameplayAttributeData(this)->GetCurrentValue();
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	else if (Attribute == GetSwimSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetRideSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetFlySpeedAttribute())
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
	else if (Attribute == GetDefendScopeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetPhysicsDefRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetMagicDefRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetToughnessRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetStaminaRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else if (Attribute == GetStaminaExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
	}
	else
	{
		Super::PreAttributeChange(Attribute, NewValue);
	}
}

void UDWCharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	AActor* TargetActor = nullptr;
	IAbilityVitalityInterface* TargetVitality = nullptr;
	ADWCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetVitality = Cast<IAbilityVitalityInterface>(TargetActor);
		TargetCharacter = Cast<ADWCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetInterruptAttribute())
	{
		if (TargetCharacter)
		{
			TargetCharacter->HandleInterrupt(GetInterrupt());
		}
		SetInterrupt(0.f);
	}
	else
	{
		Super::PostGameplayEffectExecute(Data);
	}
}
