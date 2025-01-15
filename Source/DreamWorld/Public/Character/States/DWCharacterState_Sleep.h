// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Ability/Character/States/AbilityCharacterStateBase.h"
#include "DWCharacterState_Sleep.generated.h"

class AVoxelInteractAuxiliary;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UDWCharacterState_Sleep : public UAbilityCharacterStateBase
{
	GENERATED_BODY()

	friend class ADWCharacter;

public:
	UDWCharacterState_Sleep();

public:
	virtual void OnInitialize(UFSMComponent* InFSM, int32 InStateIndex) override;

	virtual bool OnPreEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;

	virtual void OnEnter(UFiniteStateBase* InLastState, const TArray<FParameter>& InParams) override;
	
	virtual void OnRefresh(float DeltaSeconds) override;

	virtual void OnLeave(UFiniteStateBase* InNextState) override;

	virtual void OnTermination() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStats")
	AVoxelInteractAuxiliary* Bed;

public:
	AVoxelInteractAuxiliary* GetBed() const { return Bed; }
};
