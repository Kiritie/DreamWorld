#pragma once

#include "Ability/Item/Prop/AbilityPropDataBase.h"

#include "DWPropData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWPropData : public UAbilityPropDataBase
{
	GENERATED_BODY()

public:
	UDWPropData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWPropType PropType;
};
