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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* AttackAbilityTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* SkillAbilityTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* ActionAbilityTable;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDWCharacterAttackAbilityData FallingAttackAbility;
};
