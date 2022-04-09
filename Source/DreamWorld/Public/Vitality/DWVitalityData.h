#pragma once

#include "Ability/Item/ItemDataBase.h"

#include "DWVitalityData.generated.h"

class AAbilityVitalityBase;
UCLASS(BlueprintType)
class DREAMWORLD_API UDWVitalityData : public UItemDataBase
{
	GENERATED_BODY()

public:
	UDWVitalityData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventorySaveData InventoryData;
};
