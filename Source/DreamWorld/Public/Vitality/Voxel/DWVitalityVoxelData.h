#pragma once

#include "Vitality/DWVitalityData.h"

#include "DWVitalityVoxelData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWVitalityVoxelData : public UDWVitalityData
{
	GENERATED_BODY()

public:
	UDWVitalityVoxelData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPrimaryAssetId VoxelID;
};
