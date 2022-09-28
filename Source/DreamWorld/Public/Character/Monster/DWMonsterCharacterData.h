#pragma once

#include "Character/DWCharacterData.h"

#include "DWMonsterCharacterData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWMonsterCharacterData : public UDWCharacterData
{
	GENERATED_BODY()

public:
	UDWMonsterCharacterData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* AttackHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* AttackMissSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* AttackHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* AttackMissEffect;
};
