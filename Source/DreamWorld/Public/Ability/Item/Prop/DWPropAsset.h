#pragma once

#include "Ability/Item/Prop/PropAssetBase.h"

#include "DWPropAsset.generated.h"

UCLASS(BlueprintType)
class WHFRAMEWORK_API UDWPropAsset : public UPropAssetBase
{
	GENERATED_BODY()

public:
	UDWPropAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPropType PropType;
};
