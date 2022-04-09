#pragma once

#include "Ability/Item/Prop/PropDataBase.h"

#include "DWPropData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWPropData : public UPropDataBase
{
	GENERATED_BODY()

public:
	UDWPropData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWPropType PropType;
};
