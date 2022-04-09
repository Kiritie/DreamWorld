#pragma once

#include "Ability/Character/CharacterDataBase.h"

#include "DWCharacterData.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWCharacterData : public UCharacterDataBase
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventorySaveData InventoryData;
};
