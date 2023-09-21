#include "Common/DWCommonTypes.h"

#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Character/DWCharacter.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Scene/SceneModuleBPLibrary.h"

void UDWDamageHandle::HandleDamage(AActor* SourceActor, AActor* TargetActor, float DamageValue, EDamageType DamageType, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags)
{
	ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor);

	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(TargetActor);

	IAbilityVitalityInterface* TargetVitality = Cast<IAbilityVitalityInterface>(TargetActor);
	
	float SourceAttackForce = 0.f;
	float SourceAttackCritRate = 0.f;
	float SourceDefendRate = 0.f;
	float SourceDefendScope = 0.f;
	float SourcePhysicsDefRate = 0.f;
	float SourceMagicDefRate = 0.f;

	float LocalDamageDone = 0.f;
	float DefendRateDone = 0.f;
	bool bAttackCrited = false;
		
	if (SourceCharacter)
	{
		SourceAttackForce = SourceCharacter->GetAttackForce();
		SourceAttackCritRate = SourceCharacter->GetAttackCritRate();
	}
	if (TargetCharacter)
	{
		SourceDefendRate = TargetCharacter->GetDefendRate();
		SourceDefendScope = TargetCharacter->GetDefendScope();
		SourcePhysicsDefRate = TargetCharacter->GetPhysicsDefRate();
		SourceMagicDefRate = TargetCharacter->GetMagicDefRate();

		FVector DamageDirection = SourceActor->GetActorLocation() - TargetActor->GetActorLocation();
		DamageDirection.Normalize();
		if (FVector::DotProduct(DamageDirection, TargetActor->GetActorForwardVector())/* / 90.f*/ > (1.f - SourceDefendScope))
		{
			DefendRateDone = SourceDefendRate * (TargetCharacter->IsDefending() ? 1.f : 0.f);
			if(DefendRateDone > 0.f && !TargetCharacter->DoAction(EDWCharacterActionType::DefendBlock))
			{
				DefendRateDone = 0.f;
			}
		}
	}

	switch(DamageType)
	{
		case EDamageType::Physics:
		{
			bAttackCrited = FMath::FRand() <= SourceAttackCritRate;
			LocalDamageDone = SourceAttackForce * DamageValue * (1.f - SourcePhysicsDefRate) * (bAttackCrited ? 2.f : 1.f);
			break;
		}
		case EDamageType::Magic:
		{
			LocalDamageDone = DamageValue * (1.f - SourceMagicDefRate);
			break;
		}
	}

	if (DefendRateDone > 0.f)
	{
		if (TargetCharacter)
		{
			USceneModuleBPLibrary::SpawnWorldText(FString::FromInt(LocalDamageDone * DefendRateDone), FColor::Cyan, EWorldTextStyle::Normal, TargetCharacter->GetActorLocation(), FVector(20.f), TargetCharacter);
		}
		LocalDamageDone *= (1.f - DefendRateDone);
	}

	if (LocalDamageDone > 0.f)
	{
		if (TargetVitality && !TargetVitality->IsDead())
		{
			TargetVitality->HandleDamage(DamageType, LocalDamageDone, bAttackCrited, DefendRateDone > 0.f, HitResult, SourceTags, SourceActor);
		}
	}
}
