// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "AI/Base/AIControllerBase.h"
#include "Perception/AIPerceptionTypes.h"
#include "DWAIController.generated.h"

DECLARE_DELEGATE_TwoParams(FDWAIPerceptionUpdated, ADWCharacter*, bool)

/**
 * AI����������
 */
UCLASS()
class DREAMWORLD_API ADWAIController : public AAIControllerBase
{
    GENERATED_BODY()

public:
    ADWAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnTargetCharacterStateChanged(UFiniteStateBase* InFiniteState);

	UFUNCTION(BlueprintPure)
	EDWCharacterNature GetCharacterNature() const;

	UFUNCTION(BlueprintCallable)
	void SetCharacterNature(EDWCharacterNature InCharacterNature);
	
	UFUNCTION(BlueprintPure)
	float GetAttackDistance() const;

	UFUNCTION(BlueprintCallable)
	void SetAttackDistance(float InAttackDistance);
		
	UFUNCTION(BlueprintPure)
	float GetFollowDistance() const;

	UFUNCTION(BlueprintCallable)
	void SetFollowDistance(float InFollowDistance);
			
	UFUNCTION(BlueprintPure)
	float GetPatrolDistance() const;

	UFUNCTION(BlueprintCallable)
	void SetPatrolDistance(float InPatrolDistance);
				
	UFUNCTION(BlueprintPure)
	float GetPatrolDuration() const;

	UFUNCTION(BlueprintCallable)
	void SetPatrolDuration(float InPatrolDuration);

	UFUNCTION(BlueprintPure)
	bool IsLostTarget() const;
	
	UFUNCTION(BlueprintCallable)
	void SetLostTarget(bool bLostTarget, FVector InLostTargetLocation = FVector::ZeroVector);

	UFUNCTION(BlueprintPure)
	ADWCharacter* GetTargetCharacter() const;
	
	UFUNCTION(BlueprintCallable)
	void SetTargetCharacter(ADWCharacter* InTargetCharacter);
};
