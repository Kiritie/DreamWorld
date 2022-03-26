#pragma once

#include "Ability/Item/Equip/EquipAssetBase.h"

#include "DWEquipAsset.generated.h"


UCLASS(BlueprintType)
class WHFRAMEWORK_API UDWEquipAsset : public UEquipAssetBase
{
	GENERATED_BODY()

public:
	UDWEquipAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipPartType PartType;
};
