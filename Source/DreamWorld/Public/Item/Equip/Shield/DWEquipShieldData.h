#pragma once

#include "Item/Equip/DWEquipData.h"

#include "DWEquipShieldData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipShieldData : public UDWEquipData
{
	GENERATED_BODY()

public:
	UDWEquipShieldData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWShieldType ShieldType;
};
