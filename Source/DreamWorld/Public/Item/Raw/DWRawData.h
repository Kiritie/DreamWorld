#pragma once

#include "Ability/Item/Raw/AbilityRawDataBase.h"
#include "Common/DWCommonTypes.h"

#include "DWRawData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWRawData : public UAbilityRawDataBase
{
	GENERATED_BODY()

public:
	UDWRawData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWRawType RawType;
};
