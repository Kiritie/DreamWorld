#pragma once

#include "Asset/Primary/Item/ItemAssetBase.h"

#include "DWVitalityAsset.generated.h"

class AAbilityVitalityBase;
UCLASS(BlueprintType)
class DREAMWORLD_API UDWVitalityAsset : public UItemAssetBase
{
	GENERATED_BODY()

public:
	UDWVitalityAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventorySaveData InventoryData;
};
