#pragma once

#include "Ability/Item/Equip/EquipDataBase.h"

#include "DWEquipData.generated.h"


UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipData : public UEquipDataBase
{
	GENERATED_BODY()

public:
	UDWEquipData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWEquipType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWEquipPartType PartType;
};
