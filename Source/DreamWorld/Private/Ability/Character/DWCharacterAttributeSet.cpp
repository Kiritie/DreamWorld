#include "Ability/Character/DWCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Character/DWCharacter.h"

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
	
}

void UDWCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(AbilityComp->GetAvatarActor());
	
	const float CurrentValue = Attribute.GetGameplayAttributeData(this)->GetCurrentValue();
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		if (TargetCharacter)
		{
			TargetCharacter->HandleManaChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		if (TargetCharacter)
		{
			TargetCharacter->HandleMaxManaChanged(NewValue, NewValue - CurrentValue);
		}
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetMaxStaminaAttribute());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
		if (TargetCharacter)
		{
			TargetCharacter->HandleStaminaChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		if (TargetCharacter)
		{
			TargetCharacter->HandleMaxStaminaChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetSwimSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleSwimSpeedChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetRideSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleRideSpeedChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetFlySpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleFlySpeedChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetDodgeForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleDodgeForceChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetAttackForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleAttackForceChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetRepulseForceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleRepulseForceChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleAttackSpeedChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetAttackCritRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
		if (TargetCharacter)
		{
			TargetCharacter->HandleAttackCritRateChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetAttackStealRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
		if (TargetCharacter)
		{
			TargetCharacter->HandleAttackStealRateChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetDefendRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
		if (TargetCharacter)
		{
			TargetCharacter->HandleDefendRateChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetDefendScopeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
		if (TargetCharacter)
		{
			TargetCharacter->HandleDefendScopeChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetPhysicsDefRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
		if (TargetCharacter)
		{
			TargetCharacter->HandlePhysicsDefRateChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetMagicDefRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
		if (TargetCharacter)
		{
			TargetCharacter->HandleMagicDefRateChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetToughnessRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
		if (TargetCharacter)
		{
			TargetCharacter->HandleToughnessRateChanged(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetStaminaRegenSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleRegenSpeedAttribute(NewValue, NewValue - CurrentValue);
		}
	}
	else if (Attribute == GetStaminaExpendSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, NewValue);
		if (TargetCharacter)
		{
			TargetCharacter->HandleExpendSpeedAttribute(NewValue, NewValue - CurrentValue);
		}
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
	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(TargetActor);
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetVitality = Cast<IAbilityVitalityInterface>(TargetActor);
		TargetCharacter = Cast<ADWCharacter>(TargetActor);
	}
	
	if (Data.EvaluatedData.Attribute.GetName().EndsWith("Damage"))
	{
		float SourceAttackForce = 0.f;
		float SourceAttackCritRate = 0.f;
		float SourceDefendRate = 0.f;
		float SourceDefendScope = 0.f;
		float SourcePhysicsDefRate = 0.f;
		float SourceMagicDefRate = 0.f;

		float LocalDamageDone = 0.f;
		float DefendRateDone = 0.f;
		
		AActor* SourceActor = nullptr;
		ADWCharacter* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceCharacter = Cast<ADWCharacter>(SourceActor);
			if(SourceCharacter)
			{
				SourceAttackForce = SourceCharacter->GetAttackForce();
				SourceAttackCritRate = SourceCharacter->GetAttackCritRate();
			}
		}
		if (TargetCharacter)
		{
			SourceDefendRate = TargetCharacter->GetDefendRate();
			SourceDefendScope = TargetCharacter->GetDefendScope();
			SourcePhysicsDefRate = TargetCharacter->GetPhysicsDefRate();
			SourceMagicDefRate = TargetCharacter->GetMagicDefRate();

			FVector DamageDirection = SourceActor->GetActorLocation() - TargetActor->GetActorLocation();
			if (FVector::DotProduct(DamageDirection, TargetActor->GetActorForwardVector()) / 90 > (1 - SourceDefendScope))
			{
				DefendRateDone = SourceDefendRate * (TargetCharacter->IsDefending() ? 1 : 0);
				if(DefendRateDone > 0.f && !TargetCharacter->DoAction(ECharacterActionType::DefendBlock))
				{
					DefendRateDone = 0.f;
				}
			}
		}

		if(Data.EvaluatedData.Attribute.GetName().StartsWith("Physics"))
		{
			LocalDamageDone = SourceAttackForce * GetPhysicsDamage() * (1 - SourcePhysicsDefRate) * (1 - DefendRateDone) * (FMath::FRand() <= SourceAttackCritRate ? 2 : 1);
			if(SourceCharacter)
			{
				SourceCharacter->DoAction(LocalDamageDone > 0.f ? ECharacterActionType::AttackHit : ECharacterActionType::AttackMiss);
			}
			SetPhysicsDamage(0.f);
		}
		else if(Data.EvaluatedData.Attribute.GetName().StartsWith("Magic"))
		{
			LocalDamageDone = GetMagicDamage() * (1 - SourceMagicDefRate) * (1 - DefendRateDone) * (FMath::FRand() <= SourceAttackCritRate ? 2 : 1);
			SetMagicDamage(0.f);
		}

		if (LocalDamageDone > 0.f)
		{
			if (TargetVitality && !TargetVitality->IsDead())
			{
				TargetVitality->ModifyHealth(-LocalDamageDone);

				FHitResult HitResult;
				if (Context.GetHitResult())
				{
					HitResult = *Context.GetHitResult();
				}
				TargetVitality->HandleDamage(EDamageType::Physics, LocalDamageDone, true, HitResult, SourceTags, SourceActor);

				if(TargetCharacter && DefendRateDone == 0.f)
				{
					TargetCharacter->DoAction(ECharacterActionType::GetHit);
				}
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetInterruptAttribute())
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
