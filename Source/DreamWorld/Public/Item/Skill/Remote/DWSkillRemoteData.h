#pragma once

#include "Ability/Item/Skill/AbilitySkillDataBase.h"

#include "DWSkillRemoteData.generated.h"


UCLASS(BlueprintType)
class DREAMWORLD_API UDWSkillRemoteData : public UAbilitySkillDataBase
{
	GENERATED_BODY()

public:
	UDWSkillRemoteData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* AttackHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* AttackHitEffect;
};
