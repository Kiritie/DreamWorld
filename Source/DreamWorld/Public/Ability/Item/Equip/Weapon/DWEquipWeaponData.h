#pragma once

#include "Ability/Item/Equip/DWEquipData.h"
#include "Ability/Item/ItemDataBase.h"

#include "DWEquipWeaponData.generated.h"

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
};
