#include "DWTypes.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
#include "SaveGame/General/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Ability/Abilities/CharacterAbilityBase.h"
#include "Vitality/DWVitalityData.h"
#include "Character/DWCharacterData.h"
#include "Character/DWCharacter.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"

void UDWDamageHandle::HandleDamage(AActor* SourceActor, AActor* TargetActor, float DamageValue, EDamageType DamageType, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags)
{
	ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor);

	IAbilityVitalityInterface* TargetVitality = Cast<IAbilityVitalityInterface>(TargetActor);
	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(TargetActor);
	
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
		if (FVector::DotProduct(DamageDirection, TargetActor->GetActorForwardVector()) / 90 > (1 - SourceDefendScope))
		{
			DefendRateDone = SourceDefendRate * (TargetCharacter->IsDefending() ? 1 : 0);
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
			LocalDamageDone = SourceAttackForce * DamageValue * (1 - SourcePhysicsDefRate) * (1 - DefendRateDone) * (bAttackCrited ? 2 : 1);
			break;
		}
		case EDamageType::Magic:
		{
			LocalDamageDone = DamageValue * (1 - SourceMagicDefRate) * (1 - DefendRateDone);
			break;
		}
	}

	if (LocalDamageDone > 0.f)
	{
		if (TargetVitality && !TargetVitality->IsDead())
		{
			TargetVitality->HandleDamage(DamageType, LocalDamageDone, bAttackCrited, DefendRateDone > 0.f, HitResult, SourceTags, SourceActor);
		}
	}
}
