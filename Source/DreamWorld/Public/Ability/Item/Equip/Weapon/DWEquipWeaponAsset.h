#pragma once

#include "Ability/Item/Equip/DWEquipAsset.h"
#include "Asset/Primary/Item/ItemAssetBase.h"

#include "DWEquipWeaponAsset.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipWeaponAsset : public UDWEquipAsset
{
	GENERATED_BODY()

public:
	UDWEquipWeaponAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponHandType HandType;
};
