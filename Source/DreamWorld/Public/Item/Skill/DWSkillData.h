#pragma once

#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Common/DWCommonTypes.h"

#include "DWSkillData.generated.h"


UCLASS(BlueprintType)
class DREAMWORLD_API UDWSkillData : public UAbilitySkillDataBase
{
	GENERATED_BODY()

public:
	UDWSkillData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWSkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponPart WeaponPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCancelAble;

public:
	virtual FString GetItemErrorInfo(AActor* InOwner, int32 InLevel) const override;
};
