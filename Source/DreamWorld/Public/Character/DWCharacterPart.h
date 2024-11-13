// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Character/AbilityCharacterPartBase.h"
#include "Common/DWCommonTypes.h"

#include "DWCharacterPart.generated.h"

/**
 * 角色部位组件
 */
UCLASS(ClassGroup="Collision", editinlinenew, meta=(DisplayName="DW Character Part", BlueprintSpawnableComponent))
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
	EDWCharacterPart CharacterPart;

public:
	UFUNCTION(BlueprintPure)
	EDWCharacterPart GetCharacterPart() const { return CharacterPart; }

	UFUNCTION(BlueprintCallable)
	void SetCharacterPart(EDWCharacterPart InCharacterPart) { this->CharacterPart = InCharacterPart; }
};
