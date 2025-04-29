#pragma once

#include "Common/DWCommonTypes.h"
#include "Ability/Character/AbilityCharacterDataBase.h"

#include "DWCharacterData.generated.h"

class UDialogue;

UCLASS(BlueprintType)
class DREAMWORLD_API UDWCharacterData : public UAbilityCharacterDataBase
{
	GENERATED_BODY()

public:
	UDWCharacterData();

public:
	virtual void OnReset_Implementation() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityItems> TalentItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDWWeaponType, FDWCharacterAttackAbilities> AttackAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDWWeaponType, FDWCharacterFallingAttackAbilityData> FallingAttackAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDWCharacterSkillAttackAbilityData> SkillAttackAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UDialogue*> Dialogues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "(int32)Nature > 2"))
	UBehaviorTree* DefaultBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "Nature == EDWCharacterNature::AINeutral"))
	UBehaviorTree* ExcessiveBehaviorTree;

protected:
	UPROPERTY(Transient)
	int32 LocalDialogueIndex;

public:
	UFUNCTION(BlueprintCallable)
	UDialogue* GetRandomDialogue(FRandomStream RandomStream = FRandomStream());
};
