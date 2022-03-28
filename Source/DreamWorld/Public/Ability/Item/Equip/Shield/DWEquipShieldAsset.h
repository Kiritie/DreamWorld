#pragma once

#include "Ability/Item/Equip/DWEquipAsset.h"

#include "DWEquipShieldAsset.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipShieldAsset : public UDWEquipAsset
{
	GENERATED_BODY()

public:
	UDWEquipShieldAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EShieldType ShieldType;
};
