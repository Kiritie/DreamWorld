#pragma once

#include "Ability/Item/Equip/DWEquipAsset.h"

#include "DWEquipShieldAsset.generated.h"

UCLASS(BlueprintType)
class WHFRAMEWORK_API UDWEquipShieldAsset : public UDWEquipAsset
{
	GENERATED_BODY()

public:
	UDWEquipShieldAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EShieldType ShieldType;
};
