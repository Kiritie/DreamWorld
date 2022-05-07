#pragma once

#include "Character/Human/DWHumanCharacterData.h"

#include "DWPlayerCharacterData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWPlayerCharacterData : public UDWHumanCharacterData
{
	GENERATED_BODY()

public:
	UDWPlayerCharacterData();
};
