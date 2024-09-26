#pragma once

#include "DWEquipWeaponData.h"

#include "DWEquipWeaponMeleeData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipWeaponMeleeData : public UDWEquipWeaponData
{
	GENERATED_BODY()

public:
	UDWEquipWeaponMeleeData();

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
