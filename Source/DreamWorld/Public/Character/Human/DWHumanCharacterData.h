#pragma once

#include "Character/DWCharacterData.h"

#include "DWHumanCharacterData.generated.h"

class UDialogue;

UCLASS(BlueprintType)
class DREAMWORLD_API UDWHumanCharacterData : public UDWCharacterData
{
	GENERATED_BODY()

public:
	UDWHumanCharacterData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDialogue* Dialogue;
};
