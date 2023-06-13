#pragma once

#include "Global/DWGlobalTypes.h"
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag DodgingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag SprintingTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag CrouchingTag;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag SwimmingTag;
				
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag FloatingTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag ClimbingTag;
					
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag RidingTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag FlyingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag AttackingTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag NormalAttackingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag FallingAttackingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag SkillAttackingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag DefendingTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag InterruptingTag;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag ExhaustedTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag FreeToAnimTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag LockRotationTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag BreakAllInputTag;
};
