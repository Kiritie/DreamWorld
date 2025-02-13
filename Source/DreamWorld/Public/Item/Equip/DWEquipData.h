#pragma once

#include "Common/DWCommonTypes.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"

#include "DWEquipData.generated.h"


UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipData : public UAbilityEquipDataBase
{
	GENERATED_BODY()

public:
	UDWEquipData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWEquipType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWEquipPart EquipPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWEquipSocket EquipSocket;
};
