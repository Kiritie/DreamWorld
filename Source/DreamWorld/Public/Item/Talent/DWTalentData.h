#pragma once

#include "Ability/Item/AbilityItemDataBase.h"

#include "DWTalentData.generated.h"


UCLASS(BlueprintType)
class DREAMWORLD_API UDWTalentData : public UAbilityItemDataBase
{
	GENERATED_BODY()

public:
	UDWTalentData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TalentPoint;

public:
	virtual FString GetItemErrorInfo(FAbilityItem InItem) const override;
};
