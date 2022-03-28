#pragma once

#include "Ability/Item/Equip/DWEquipAsset.h"
#include "DWEquipArmorAsset.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipArmorAsset : public UDWEquipAsset
{
	GENERATED_BODY()

public:
	UDWEquipArmorAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillMode SkillMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAbilitySkillBase> SkillClass;
};
