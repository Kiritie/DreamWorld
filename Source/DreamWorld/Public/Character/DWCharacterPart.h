// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Character/AbilityCharacterPartBase.h"
#include "Common/DWCommonTypes.h"

#include "DWCharacterPart.generated.h"

/**
 * 角色部位组件
 */
UCLASS()
class DREAMWORLD_API UDWCharacterPart : public UAbilityCharacterPartBase
{
	GENERATED_BODY()
	
public:
	UDWCharacterPart(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnHitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult) override;

	virtual void OnEnterVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult) override;

	virtual void OnStayVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult) override;

	virtual void OnExitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EDWCharacterPartType CharacterPartType;

public:
	UFUNCTION(BlueprintPure)
	EDWCharacterPartType GetCharacterPartType() const { return CharacterPartType; }

	UFUNCTION(BlueprintCallable)
	void SetCharacterPartType(EDWCharacterPartType InCharacterPartType) { this->CharacterPartType = InCharacterPartType; }
};
