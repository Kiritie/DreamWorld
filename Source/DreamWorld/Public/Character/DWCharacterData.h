#pragma once

#include "Common/DWCommonTypes.h"
#include "Ability/Character/AbilityCharacterDataBase.h"

#include "DWCharacterData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWCharacterData : public UAbilityCharacterDataBase
{
	GENERATED_BODY()

public:
	UDWCharacterData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWCharacterNature Nature;
			
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractDistance;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FollowDistance;
			
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolDistance;
			
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = "WeaponType"))
	TArray<FDWCharacterAttackAbilityData> AttackAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = "WeaponType"))
	TArray<FDWCharacterSkillAbilityData> SkillAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDWCharacterAttackAbilityData FallingAttackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "(int32)Nature > 2"))
	UBehaviorTree* DefaultBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "Nature == EDWCharacterNature::AINeutral"))
	UBehaviorTree* ExcessiveBehaviorTree;
};
