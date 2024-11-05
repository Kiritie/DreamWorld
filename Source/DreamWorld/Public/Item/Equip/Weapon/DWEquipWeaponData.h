#pragma once

#include "Item/Equip/DWEquipData.h"

#include "DWEquipWeaponData.generated.h"

class AAbilityProjectileBase;

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipWeaponData : public UDWEquipData
{
	GENERATED_BODY()

public:
	UDWEquipWeaponData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponHandType HandType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAbilityProjectileBase> ProjectileClass;
};
