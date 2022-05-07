#pragma once

#include "Ability/Vitality/VitalityDataBase.h"

#include "DWVitalityData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWVitalityData : public UVitalityDataBase
{
	GENERATED_BODY()

public:
	UDWVitalityData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventorySaveData InventoryData;
};
