#pragma once

#include "Item/Equip/DWEquipData.h"
#include "DWEquipArmorData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipArmorData : public UDWEquipData
{
	GENERATED_BODY()

public:
	UDWEquipArmorData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillMode SkillMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAbilitySkillBase> SkillClass;
};
