#pragma once

#include "Ability/Character/CharacterAssetBase.h"

#include "DWCharacterAsset.generated.h"

UCLASS(BlueprintType)
class DREAMWORLD_API UDWCharacterAsset : public UCharacterAssetBase
{
	GENERATED_BODY()

public:
	UDWCharacterAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterNature Nature;
			
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
